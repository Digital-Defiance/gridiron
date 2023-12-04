/****************************************************************************************
 * (C) Copyright 2009-2023
 *    Jessica Mulein <jessica@digitaldefiance.org>
 *    Digital Defiance and Contributors <https://digitaldefiance.org>
 *
 * Others will be credited if more developers join.
 *
 * License
 *
 * This code is licensed under the GPLv3.
 * Please see COPYING in the root of this package for details.
 *
 * The following libraries are only linked in, and no code is based directly from them:
 * htmlcxx is under the Apache 2.0 License
 ****************************************************************************************
 * Page Class
 * ------------------
 *
 * All controls on a page must register with this.
 * Handles parsing and rendering of control classes
 * All objects on a page must register with this one
 *
 * Most control class derivatives expect to find their data in an html node
 * supplied by a Page class's parsing operation.
 ***************************************************************************************/

#include <iostream>
#include <fstream>
#include <typeinfo>
#include <gridiron/base_classes/page.hpp>
#include <gridiron/gridiron.hpp>
#include <gridiron/exceptions.hpp>

using namespace GridIron;

static const std::string Page::Namespace = GRIDIRON_XHTML_NS;
static const std::string Page::ControlTag = "Page";

Page::Page(std::string frontPageFile) : Control(frontPageFile, NULL) : _renderTag{"html"}
{
    int filesize = 0;
    int bytesread = 0;
    int i = 0;

    // save name for access
    if (!frontPageFile.empty())
        _htmlFile = frontPageFile;
    else
        _htmlFile = std::string("::memory:" + addressof(this));

    // make up an id until we parse and match up with one
    _id = std::string(getNamespace() + "::Page" + _htmlFile); // default id = "_Page_" or "_Page_foobar.html"
    _viewStateEnabled = false;                                // whether to output the viewstate
    _autonomous = Page::AllowAutonomous();                    // not applicable, page classes cannot be autonomous

    if (frontPageFile.empty())
    {
        // if no front-page is given, the caller will have to render everything themselves
        // this is fine, but we're done here.
        return;
    }

    // open the front-page
    const std::string fullPagePath = GridIron::pathToPage(frontPage);
    std::ifstream file(fullPagePath, std::ios_base::in);
    if (!file.is_open())
        throw GridException(101, std::string("unable to open front-end page: ").append(fullPagePath).c_str());
    _htmlFilepath = fullPagePath;

    // base (control) class does not attempt to parse at all

    // get length
    file.seekg(0, std::ios_base::end);
    filesize = file.tellg();
    file.seekg(0, std::ios_base::beg);

    if (filesize == 0)
    {
        file.close();
        throw GridException(103, "front-end file is empty");
    }

    // allocate mem
    std::string buffer;
    buffer.reserve(filesize);
    while (bytesread < filesize)
    {
        char buf[filesize - bytesread];
        file.read(buf, filesize - bytesread);
        const size_t gcount = file.gcount();
        if (gcount == 0)
            break;
        bytesread += gcount;
        buffer.append(buf, gcount);
    }

    // close the file
    file.close();

    // copy buffer to our store
    _data.swap(buffer);

    if (bytesread < filesize)
    {
        throw GridException(104, "unable to read front-end page");
    }

    // add default registered variables
    _regvars[GRIDIRON_XHTML_NS + ".frontPage"] = &_htmlFilepath;
    _regvars[GRIDIRON_XHTML_NS + ".frontPageFile"] = &_htmlFile;

    // we sort of have a problem here. _namespace is constant. We don't want it to change
    // but we can't make the right hand side of the  map constant
    // arguably _htmlFile should be a constant too.
    // do we need to change the type of the right hand side to handle constant and non constant
    // types with some sort of wrapper class?

    //_regvars["__namespace"] = &_namespace;
}

unique_page_ptr Page::This()
{
    return std::unique_ptr<Page>(this);
}

Page::~Page()
{
}

std::string Page::pathToPage(std::string frontPage)
{
    std::filesystem::path basePath = std::filesystem::current_path();
    return basePath.append(GRIDIRON_HTML_DOCROOT).append(frontPage);
}

std::string Page::pathToPage()
{
    return this;
}

// TODO: all the std::cerr's are either debug printing or need to be converted to throws
//
// This function uses htmlcxx to parse the html front page into a node tree
//
// Parsing happens in two passes- one automatically at instantiation of the page that searches for autos
// and the second when render is called.
// if the tree object has already been filled in, we know this is the second (or later) pass
Page *
Page::fromHtmlNode(htmlnode &node)
{
    int controlcount = 0;                 // how many custom controls we find
    bool firstpass = (_tree.size() == 0); // have we already parsed this page?

    // _data contains the entire .html file, if given
    if (_data.size() == 0)
        throw GridException(105, "parse called when front-end page not given or empty");

    // now get the htmlcxx parse tree of the page if we need it
    if (firstpass)
    {
        htmlcxx::HTML::ParserDom parser;
        parser.parse(_data.toStdString());
        _tree = parser.getTree();
    }

    std::cerr << std::endl
              << (firstpass ? "First " : "Second ") << "parsing pass starting" << std::endl;

    // now go through the tags on the page looking only for gridiron auto tags at instantiation
    // if not firstpass, ignore autos and look for regular tags, then search instantiated controls for one with the correct id
    tree<htmlnode>::iterator it = _tree.begin();
    tree<htmlnode>::iterator end = _tree.end();
    while (it != end)
    {
        // tags we're interested in are <gridiron::* id="foo"></gridiron::*>
        // a quirk of htmlcxx is helpful to us, tagName only has up to the ::'s

        // if the current element is a Tag (not a comment, etc)
        // and it's one we're supposed to interpret:
        if (it->isTag())
        {
            std::pair<std::string, std::string> frameworkTag = GridIron::gridironParseTag(it->tagName());
            if (!frameworkTag.first.empty())
            {
                // get the full Tag string
                std::string tagData = it->text();

                if (tagType.empty())
                {
                    std::cerr << "ERROR: Control Tag is missing control type" << std::endl;
                }
                else
                {
                    // use the htmlcxx parsing routine to get all of the attributes and values
                    it->parseAttributes();

                    // the first part of the result pair indicates whether the attribute was found
                    // the second has the actual id, if present
                    std::pair<bool, std::string> idresult = it->attribute("id");
                    if (!idresult.first)
                    {
                        std::cerr << "ERROR: Control Tag is missing id" << std::endl;
                    }
                    else
                    {
                        // same story here, bool will be true if Tag contained auto
                        std::pair<bool, std::string> autoresult = it->attribute("auto");
                        bool isauto = (autoresult.first && (autoresult.second == "true"));

                        // look for any controls on the page with specified ID
                        Control *instance = FindByID(idresult.second);

                        // if we found an auto Tag and it's the first pass, and the id was already registered (earlier in the while loop, by another Tag)
                        if ((instance != NULL) && isauto && firstpass)
                        {
                            std::cerr << "found auto Tag (" << tagData
                                      << ") and the specified ID was already in use by a control of type "
                                      << instance->fullName() << std::endl;

                            // if we found a standard Tag, the id was registered (as it should be, by the client code) and it's not the first pass
                        }
                        else if ((instance != NULL) && !isauto && !firstpass)
                        {
                            std::cerr << "found Tag (" << tagData << ") and instance with type " << instance->fullName()
                                      << ", id=" << idresult.second << std::endl;

                            // make sure the instance with that ID is the same type as the control Tag
                            if (!(Control::instanceOf<Control>(instance)))
                            {
                                std::cerr << "ERROR: instance with that id is not a " << tagType << std::endl;
                            }
                            else
                            {
                                // if it's the right type and id, but it already has an html node associated, we've already seen this Tag in the file- duplicate
                                if (instance->HTMLNodeRegistered())
                                {
                                    std::cerr << "Control instance already bound to another Tag." << std::endl;

                                    // otherwise, we've found the instance that's supposed to match this Tag
                                }
                                else
                                {
                                    std::cerr << "Control Tag and instance match up." << std::endl;
                                    // set the associated node pointer
                                    instance->SetHTMLNode(&(*it));
                                    // add to nodemap
                                    _nodemap[&(*it)] = instance;
                                    // count how many controls we found
                                    controlcount++;
                                }
                            }

                            // if we found an auto Tag on the first pass and no one is using the specified id (at this point instance is guaranteed == NULL, given the other two)
                            // we've previously handled (instance != NULL, auto, firstpass) and (instance != NULL, !auto, !firstpass)
                            // if auto and firstpass, instance has to be NULL- meaning the Tag's requested id is available
                        }
                        else if (isauto && firstpass)
                        {
                            std::cerr << "found auto Tag, specified ID is available" << std::endl;

                            // try to create a control of this type. The control class must be registered with the factory.
                            // only classes that support autos should register.
                            instance = globalControlFactory.CreateByType(tagType.c_str(), idresult.second.c_str(),
                                                                         (Control *)this);

                            // If we get an instance, it worked, if it didn't tough luck.
                            if (instance == NULL)
                            {
                                std::cerr << "unable to create autonomous control of type " << tagType << std::endl;
                            }
                            else
                            {
                                std::cerr << "autonomous Tag of type=" << tagType << ", id=" << idresult.second
                                          << " was created." << std::endl;
                                // set the associated node pointer
                                instance->SetHTMLNode(&(*it));
                                // add to nodemap
                                _nodemap[&(*it)] = instance;
                                // add to the count of registered controls
                                controlcount++;
                            }

                            // if still auto Tag, by elimination, this isn't the first pass- we're not interested. No error here.
                        }
                        else if (isauto)
                        {
                            std::cerr << "found auto Tag, skipping for second pass" << std::endl;
                        }
                    }
                }
            }
        }
        ++it;
    }
    std::cerr << (firstpass ? "First " : "Second ") << "parsing pass complete." << std::endl
              << std::endl;
}

// helper class to output recursively render all tags we're in control of and output the stuff in the template inbetween
// called by render
void Page::renderNode(tree<htmlnode>::sibling_iterator *thisnode, int level, folly::fbstring &data)
{
    tree<htmlnode>::sibling_iterator sib = thisnode->begin();
    while (sib != thisnode->end())
    {
        htmlnode *tmpnode = &(sib.node->data);

        if (tmpnode->isTag() && (tmpnode->tagName() == Page::_namespace))
        {
            // retrieve the control instance using the html node instance
            Control *tmpcontrol = _nodemap[tmpnode];

            // if we found the control associated with this node, tell it to render
            // otherwise, print an error in its place
            if (tmpcontrol != NULL)
                tmpcontrol->render(data);
            else
                data.append("<!-- ERROR rendering control: no instance found -->");
        }
        else
        {
            // otherwise, stick in the html
            data.append(sib->text());
            // any children of this node
            if (sib->isTag())
                renderNode(&sib, level + 1, data);
            // and the stuff after
            data.append(sib->closingText());
        }

        ++sib; // next!
    }
}

// NOTE: if a custom control can have children, it's up to that control to implement the recursive rendering
std::ostream &operator<<(std::ostream &os, const Control &control)
{
    int lastpos = 0, startpos = 0, endpos = -1, testpos = -1, i = 0;
    bool replace = false;
    std::string *datacontents;
    var_map::iterator m;

    if (_data.size() == 0)
        throw GridException(104, "render called when front-end page not given or empty");
    this->Page::parse(); // call 2nd pass

    // assemble page: start by rendering the first node.
    tree<htmlnode>::sibling_iterator sib = _tree.begin();
    renderNode(&sib, 1, data);
}

// for controls to make variables available for HTML replacement. alphanumeric and _ only.
bool Page::RegisterVariable(const std::string name, std::string *data)
{
    // NOTE: tags starting with __ should be system generated vars only, but we won't check

    // make sure name has a length
    if (name.length() == 0)
        return false;

    // validate name characters
    int pos = name.find_first_not_of("_-abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789."); // allow dots
    if (pos != std::string::npos)
        return false;

    // search through variables, make sure name isn't already registered
    for (var_map::iterator m = _regvars.begin(); m != _regvars.end(); ++m)
    {
        if (m->first == name)
            return false;
    }

    // no existing var with same name found, register it
    _regvars[name] = data;
    return true;
}
