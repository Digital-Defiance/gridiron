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

#include <gridiron/gridiron.hpp>
#include <gridiron/controls/Control.hpp>

namespace GridIron {
    class Page;

    Control::Control(const char *id, std::shared_ptr<Control> parent) :
        ID{id},
        Parent{parent}
    {
        this->tagName_ = std::string(GRIDIRON_XHTML_NS).append("::Control");
        // we must have an ID- and only one instance of an id may exist on all controls under a page object
        if (ID.get().length() == 0) throw GridException(200, "no id specified");

        // find the page control if we have one, then look to see if the id is already registered
        std::shared_ptr<Control> _Page = GetPage();
        if (_Page != nullptr) {
            // check page for existing controls with that id
            std::shared_ptr<Control> result = _Page.get()->FindByID(ID, true);
            if (result != nullptr) throw GridException(201, "id already in use");

            // register ourselves with the parent if we have one (pages dont)
            // parent will have a pointer to our id string to save mem and allow for changes
            if (Parent.get() != nullptr) Parent.get()->registerChild(ID.get(), This());
        }
    }

    std::ostream& operator<<(std::ostream& os, const Control& control) {
        os << control;
        return os;
    }

    // find the bottom-most control, regardless of type
    // returns: pointer - may be self
    std::shared_ptr<Control>
    Control::GetRoot(void) {
        std::shared_ptr<Control> ptr = this->This;

        while (ptr->Parent.get() != nullptr) {
            ptr = ptr->Parent.get();
        }

        return ptr;
    }

    // find the bottom-most control, only if a Page object
    // returns: pointer on success or nullptr, may be self
    std::shared_ptr<Control>
    Control::GetPage(bool rootOnly) {
        std::shared_ptr<Control> ptr = This;

        // short circuit if already on page and either not rootOnly search or our parent is null (we're root)
        if (GridIron::instanceOf<Control, Page>(ptr) && (!rootOnly || !ptr->Parent())) {
            return std::dynamic_pointer_cast<std::shared_ptr<Page>>(This);
        } else if (!ptr->Parent()) {
            // if no parent left to check either, we can just return now
            return nullptr;
        }

        // recursive call
        return This()->Parent()->GetPage(readOnly);
    }

    std::shared_ptr<Control>
    Control::Find(Control& control) {
        // check map
        std::map<Control*, std::shared_ptr<Control>>::iterator it = _controlsByControl.find(&control);
        if (it != _controlsByControl.end()) {
            return it->first->This;
        }
        // not found, create a shared pointer
        std::shared_ptr<Control> spC = std::shared_ptr<Control> (&control);
        // add to map
        std::pair<std::map<Control*, std::shared_ptr<Control>>::iterator,bool> inserted = _controlsByControl.insert(std::make_pair(&control,std::move(spC)));
        // return the shared pointer
        return inserted.first->first->This;
    }

    // recursive search through all controls under this object, return the one with specified id
    std::shared_ptr<Control>
    Control::FindByID(const std::string &id, bool searchParentsIfNotChild) {

        // prioritize children first. if the end user wants to be efficient for a tree-wide search, start at root
        if (_children.size() > 0) {
            // search immediate children first
            for (std::vector<std::shared_ptr<Control>>::iterator it = _children.begin(); it != _children.end(); ++it) {
                // check the child's id first
                if ((*it)->ID.get().compare(id) == 0) {
                    return (*it)->This;
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
        os << this->Namespace << "::" << this->RenderTag;
        return os;
    }

    std::string Control::NamespacedTag() {
        return (std::string(this->Namespace).append("::").append(this->RenderTag));
    }

    // register this control with the parent
    bool
    Control::registerChild(std::string id, std::shared_ptr<Control> control) {
        if (id.empty() || (control == nullptr)) return false;

        // check for duplicates first
        for (std::vector<std::shared_ptr<Control>> ::iterator it = _children.begin(); it != _children.end(); ++it) {
            if ((*it)->ID() == id) return false;    // id already in list
        }


        // then add
        _children.push_back(control->This);
        std::cerr << "There are now " << _children.size() << " children registered." << std::endl;

        return true;
    }

    // unregister this control from the parent
    bool
    Control::unregister_child(std::string id) {
        for (std::vector<std::shared_ptr<Control>>::iterator i = _children.begin(), iend = _children.end(); i != iend; ++i) {
            if (i->get()->ID() == id) {
                _children.erase(i);
                return true;
            }
        }
        return false;
    }

    // destructor
    Control::~Control() {
        // unregister ourselves from the parent if we have one (pages dont)
        if (nullptr == Parent.get()) Parent.get()->unregister_child(ID);
    }

    // allow the page class to tell us we're an anonymous instance
    void
    Control::EnableAutonomous() {
        if (this->AllowAutonomous) {
            this->Autonomous = ROProperty<bool>(true);
        }
    }

#if FALSE
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
#endif

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
