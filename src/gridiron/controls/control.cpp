/****************************************************************************************
 * (C) Copyright 2009-2020
 *    Jessica Mulein <jessica@mulein.com>
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

#include <gridiron/controls/control.hpp>

namespace GridIron {

    Control::Control(const char *id, std::shared_ptr<Control> parent) :
        This{std::shared_from_this()},
        Namespace{GRIDIRON_XHTML_NS},
        tagName_{GRIDIRON_XHTML_NS + "::Control"},
        RenderTag{"div"}
    {
        std::shared_ptr<Control> result = nullptr;
        std::shared_ptr<Page> _Page;

        // INITIALIZE VARIABLES
        // our id
        _id = std::string(id);
        // our parent, as given
        _parent = parent;
        // whether this page should be serialized into the viewstate
        _viewStateEnabled = false;
        // whether this is an autonomous control - affects behavior in derived classes
        _autonomous = false;

        // we must have an ID- and only one instance of an id may exist on all controls under a page object
        if (_id.length() == 0) throw GridException(200, "no id specified");

        // find the page control if we have one, then look to see if the id is already registered
        _Page = GetPage();
        if (_Page != nullptr) {
            // check page for existing controls with that id
            result = _Page->FindByID(_id, true);
            if (result != nullptr) throw GridException(201, "id already in use");

            // register ourselves with the parent if we have one (pages dont)
            // parent will have a pointer to our id string to save mem and allow for changes
            if (_parent != nullptr) _parent->registerChild(_id, this);
        }
    }

    friend std::ostream& operator<<(std::ostream& os, const Control& control) {
        os << control.HtmlNode();
        return os;
    }

    // find the bottom-most control, regardless of type
    // returns: pointer - may be self
    std::shared_ptr<Control>
    Control::GetRoot(void) {
        std::shared_ptr<Control> ptr = This();

        while (ptr->_parent != nullptr) {
            ptr = ptr->_parent->This();
        }

        return ptr;
    }

    // fine the bottom-most control, only if a Page object
    // returns: pointer on success or nullptr, may be self
    std::shared_ptr<Control>
    Control::GetPage(void) {
        std::shared_ptr<Control> ptr = GetRoot();
        if (ptr->instanceOf<Page>()) {
            return static_cast<std::shared_ptr<Page>>(ptr);
        }
        return nullptr;
    }

    std::shared_ptr<Control>
    Control::Find(Control& control) {
        // check map
        std::map<Control*, std::shared_ptr<Control>>::iterator it = _controlsByControl.find(&control);
        if (it != _controlsByControl.end()) {
            return it->first->This();
        }
        // not found, create a shared pointer
        std::shared_ptr<Control> spC = std::shared_ptr<Control> (&control);
        // add to map
        std::pair<std::map<Control*, std::shared_ptr<Control>>::iterator,bool> inserted = _controlsByControl.insert(std::make_pair(&control,std::move(spC)));
        // return the shared pointer
        return inserted.first->first->This();
    }

    // recursive search through all controls under this object, return the one with specified id
    std::shared_ptr<Control>
    Control::FindByID(const std::string id, bool searchParentsIfNotChild) {

        // prioritize children first. if the end user wants to be efficient for a tree-wide search, start at root
        if (_children.size() > 0) {
            // search immediate children first
            for (std::vector<std::shared_ptr<Control>>::iterator it = _children.begin(); it != _children.end(); ++it) {
                // check the child's id first
                if ((**it).ID() == id) {
                    return (std::static_pointer_cast<std::shared_ptr<Control>>(*it));
                }
            }
        }

        if (searchParentsIfNotChild) {
            std::map<std::string, std::shared_ptr<Control>>::iterator it = _controlsByID.find(id);
            if (it != _controlsByID.end()) {
                return it->second;
            }
        }

        // no luck
        return nullptr;
    }

    std::ostream &Control::NamespacedTag(std::ostream &os) {
        os << this->Namespace << "::" << this->Tag;
        return os;
    }

    std::string Control::NamespacedTag() {
        return (this->Namespace + "::" + this->Tag)
    }

    // register this control with the parent
    bool
    Control::registerChild(std::string id, Control *control) {
        if (id.empty() || (control == nullptr)) return false;

        // check for duplicates first
        for (std::vector<std::shared_ptr<Control>> ::iterator it = _children.begin(); it != _children.end(); ++it) {
            if ((*it)->ID() == id) return false;    // id already in list
        }


        // then add
        std::shared_ptr<Control> spC = std::make_shared<Control>(control);
        _children.push_back(new std::shared_ptr<Control>(control));
        std::cerr << "There are now " << _children.size() << " children registered." << std::endl;

        return true;
    }

    // unregister this control from the parent
    bool
    Control::unregister_child(std::string &id) {
        for (std::map<std::string, std::shared_ptr<Control>>::iterator i = _children.begin(), iend = _children.end(); i != iend; ++i) {
            if (*(i->first) == id) {
                _children.erase(i);
                return true;
            }
        }
        return false;

    }

    // destructor
    Control::~Control() {
        // unregister ourselves from the parent if we have one (pages dont)
        if (_parent != nullptr) _parent->unregister_child(_id);
    }

    // allow the page class to tell us we're an anonymous instance
    void
    Control::SetAutonomous(bool isauto) {
        _autonomous = isauto;
    }

    static std::shared_ptr<Control> Control::fromHtmlNode(htmlcxx2::HTML::Node &node) {
        _Page = GetPage();
        if (_Page == nullptr) throw GridException(300, "Control must be attached to a page");
        if (node == nullptr) throw GridException(301, "HTML Tag not found for this instance");
        Control c = Control();

        // if there are child nodes, that will be the text for the label, should not override any text that has already been set.
        // if _defaulttext == true, can override
        // a shortcut to the below might be to read the parsed sibling/child

        // only parse the original/default text if we need it (it hasn't been changed)
        if (_defaulttext) {

            std::string starttag = node->text();
            std::string endtag = node->closingText();

            _text = data->substr(node->offset() + starttag.length(),
                                 node->length() - endtag.length() - starttag.length());
        }

        // if we're an autonomous Tag, automatically register the text string for access
        // otherwise client will have to manually register if they want it accessible
        if (_autonomous) _Page->RegisterVariable(_id + ".Text", &_text);
    }

    // ------------------------------------------------
    // The following are used to allow us to instantiate control classes by type name as autonomous controls
    // Derived from Dr. Dobbs http://www.ddj.com/184410633
    ControlFactory::ControlFactory() {
        // MUST be empty- see article
    }

    ControlFactory::~ControlFactory() {
        // individual entries need not be deleted
        delete _controlProxies;
    }

    // if the list of proxies is empty, create it
    void
    ControlFactory::Init() {
        if (!_controlProxies) _controlProxies = new factory_vector;
    }

    // the derived control classes' .cpp files instantiate
    // a proxy class, which calls this, effectively adding themselves
    void
    ControlFactory::Register(const ControlFactoryProxyBase *proxy) {
        if (proxy == nullptr) return;
        Init();
        _controlProxies->push_back(proxy);
    }

    // how many class types are registered
    int
    ControlFactory::GetCount() {
        if (!_controlProxies) return 0;
        else return _controlProxies->size();
    }

    // wrapper to get access
    const
    ControlFactoryProxyBase *
    ControlFactory::GetAt(int i) {
        return _controlProxies->at(i);
    }

    // iterates through the control types that have registered, tells the proxy to create us one if its type matches
    std::shared_ptr<Control>
    ControlFactory::CreateByType(const char *type, const char *id, std::shared_ptr<Control> parent) {
        std::cerr << "Requesting creation of type " << type << std::endl;
        for (int i = 0; i < _controlProxies->size(); ++i) {
            if (strcmp(_controlProxies->at(i)->GetType(), type) == 0) {
                return _controlProxies->at(i)->CreateObject(id, parent);
            }
        }
        return nullptr;
    }

    // global factory instance
    ControlFactory globalControlFactory;
}

