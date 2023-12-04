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
 ***************************************************************************************
 * Base Control Class
 * ------------------
 *
 * All objects on a page must derive from this, and is not able to be instantiated.
 *
 * Most control class derivatives expect to find their data in an html node
 * supplied by a Page class's parsing operation.
 ***************************************************************************************/

#include <gridiron/base_classes/page.hpp>
#include <gridiron/base_classes/controls/control.hpp>

namespace GridIron
{

    Control::Control(const char *id, unique_control_ptr parent)
    {
        unique_control_ptr result = nullptr;
        unique_page_ptr _Page;

        // INITIALIZE VARIABLES
        // our id
        _id = std::string(id);
        // our parent, as given
        _parent.swap(parent);
        // whether this page should be serialized into the viewstate
        _viewStateEnabled = false;
        // whether this is an autonomous control - affects behavior in derived classes
        _autonomous = false;
        // this is a pointer to the html Tag associated with this instance
        _htmlNode = nullptr;
        _controlTagName = "Control";
        _renderTagName = "div"; // default to div

        // we must have an ID- and only one instance of an id may exist on all controls under a page object
        if (_id.length() == 0)
            throw GridException(200, "no id specified");

        // find the page control if we have one, then look to see if the id is already registered
        _Page = GetPage();
        if (_Page != nullptr)
        {
            // check page for existing controls with that id
            result = _Page->FindByID(_id, true);
            if (result != nullptr)
                throw GridException(201, "id already in use");

            // register ourselves with the parent if we have one (pages dont)
            // parent will have a pointer to our id string to save mem and allow for changes
            if (_parent != nullptr)
                _parent->registerChild(_id, this);
        }
    }

    std::ostream &operator<<(std::ostream &os, const Control &control)
    {
        os << control._htmlNode;
        return os;
    }

    unique_control_ptr Control::This()
    {
        return std::unique_ptr<Control>(this);
    }

    // find the bottom-most control, regardless of type
    // returns: pointer - may be self
    unique_control_ptr
    Control::GetRoot(void)
    {
        unique_control_ptr ptr = This();

        while (ptr->_parent != nullptr)
        {
            return _parent->This();
        }

        return ptr;
    }

    // fine the bottom-most control, only if a Page object
    // returns: pointer on success or nullptr, may be self
    unique_page_ptr
    Control::GetPage(void)
    {
        unique_control_ptr ptr = GetRoot();
        Control *p = ptr.get();
        if (!Page::instanceOf<Page>(p))
            return nullptr;
        else
            return ((Page *)p)->This();
    }

    unique_control_ptr
    Control::Find(Control &control)
    {
        // check map
        std::map<Control *, unique_control_ptr>::iterator it = _controlsByControl.find(&control);
        if (it != _controlsByControl.end())
        {
            return it->first->This();
        }
        // not found, create a shared pointer
        unique_control_ptr spC = unique_control_ptr(&control);
        // add to map
        std::pair<std::map<Control *, unique_control_ptr>::iterator, bool> inserted = _controlsByControl.insert(std::make_pair(&control, std::move(spC)));
        // return the unique pointer
        return inserted.first->first->This();
    }

    // recursive search through all controls under this object, return the one with specified id
    unique_control_ptr
    Control::FindByID(const std::string id, bool searchParentsIfNotChild)
    {

        // prioritize children first. if the end user wants to be efficient for a tree-wide search, start at root
        if (_children.size() > 0)
        {
            // search immediate children first
            for (vector_control_children::iterator it = _children.begin(); it != _children.end(); ++it)
            {
                // check the child's id first
                if ((**it).ID() == id)
                {
                    return (std::static_pointer_cast<unique_control_ptr>(*it));
                }
            }
        }

        if (searchParentsIfNotChild)
        {
            control_map::iterator it = _controlsByID.find(id);
            if (it != _controlsByID.end())
            {
                return it->second;
            }
        }

        // no luck
        return nullptr;
    }

    std::ostream &Control::fullName(std::ostream &os)
    {
        os << this->getNamespace() << "::" << this->_controlTagName;
        return os;
    }

    std::string Control::fullName()
    {
        return (this->getNamespace() + "::" + this->_controlTagName)
    }

    // register this control with the parent
    bool
    Control::registerChild(std::string id, Control *control)
    {
        if (id.empty() || (control == nullptr))
            return false;

        // check for duplicates first
        for (vector_control_children ::iterator it = _children.begin(); it != _children.end(); ++it)
        {
            if ((*it)->ID() == id)
                return false; // id already in list
        }

        // then add
        unique_control_ptr spC = std::make_shared<Control>(control);
        _children.push_back(new std::shared_ptr<Control>(control));
        std::cerr << "There are now " << _children.size() << " children registered." << std::endl;

        return true;
    }

    // unregister this control from the parent
    bool
    Control::unregister_child(std::string &id)
    {
        for (control_map::iterator i = _children.begin(), iend = _children.end(); i != iend; ++i)
        {
            if (*(i->first) == id)
            {
                _children.erase(i);
                return true;
            }
        }
        return false;
    }

    // destructor
    Control::~Control()
    {
        // unregister ourselves from the parent if we have one (pages dont)
        if (_parent != nullptr)
            _parent->unregister_child(_id);
    }

    // allow page class to tell us where our data is
    void
    Control::SetHTMLNode(htmlnode *node)
    {
        if ((_htmlNode != nullptr) && (node != nullptr))
        {
            throw GridException(500, "HTMLNode already associated");
        }
        _htmlNode = node;
    }

    // allow the page class to tell us we're an anonymous instance
    void
    Control::SetAutonomous(bool isauto)
    {
        _autonomous = isauto;
    }

    static unique_control_ptr Control::fromHtmlNode(htmlnode &node)
    {
        unique_page_ptr _Page = GetPage();
        if (_Page == nullptr)
            throw GridException(300, "Control must be attached to a page");
        if (node == nullptr)
            throw GridException(301, "HTML Tag not found for this instance");
        Control c = Control();

        // if there are child nodes, that will be the text for the label, should not override any text that has already been set.
        // if _defaulttext == true, can override
        // a shortcut to the below might be to read the parsed sibling/child

        // only parse the original/default text if we need it (it hasn't been changed)
        if (_defaulttext)
        {

            std::string starttag = _htmlNode->text();
            std::string endtag = _htmlNode->closingText();

            _text = data->substr(_htmlNode->offset() + starttag.length(),
                                 _htmlNode->length() - endtag.length() - starttag.length());
        }

        // if we're an autonomous Tag, automatically register the text string for access
        // otherwise client will have to manually register if they want it accessible
        if (_autonomous)
            _Page->RegisterVariable(_id + ".Text", &_text);
    }

    // ------------------------------------------------
    // The following are used to allow us to instantiate control classes by type name as autonomous controls
    // Derived from Dr. Dobbs http://www.ddj.com/184410633
    ControlFactory::ControlFactory()
    {
        // MUST be empty- see article
    }

    ControlFactory::~ControlFactory()
    {
        // individual entries need not be deleted
        delete _controlProxies;
    }

    // if the list of proxies is empty, create it
    void
    ControlFactory::Init()
    {
        if (!_controlProxies)
            _controlProxies = new factory_vector;
    }

    // the derived control classes' .cpp files instantiate
    // a proxy class, which calls this, effectively adding themselves
    void
    ControlFactory::Register(const ControlFactoryProxyBase *proxy)
    {
        if (proxy == nullptr)
            return;
        Init();
        _controlProxies->push_back(proxy);
    }

    // how many class types are registered
    int
    ControlFactory::GetCount()
    {
        if (!_controlProxies)
            return 0;
        else
            return _controlProxies->size();
    }

    // wrapper to get access
    const ControlFactoryProxyBase *
    ControlFactory::GetAt(int i)
    {
        return _controlProxies->at(i);
    }

    // iterates through the control types that have registered, tells the proxy to create us one if its type matches
    Control *
    ControlFactory::CreateByType(const char *type, const char *id, Control *parent)
    {
        std::cerr << "Requesting creation of type " << type << std::endl;
        for (int i = 0; i < _controlProxies->size(); ++i)
        {
            if (strcmp(_controlProxies->at(i)->GetType(), type) == 0)
            {
                return _controlProxies->at(i)->CreateObject(id, parent);
            }
        }
        return nullptr;
    }

    // global factory instance
    ControlFactory globalControlFactory;
}
