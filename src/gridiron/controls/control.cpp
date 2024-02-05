/****************************************************************************************
 * (C) Copyright 2009-2024
 *    Jessica Mulein <jessica@digitaldefiance.org>
 *    Digital Defiance and Contributors <https://digitaldefiance.org>
 *
 * Others will be credited if more developers join.
 *
 * License
 *
 * This code is licensed under the Apache license.
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

#include <algorithm>
#include <gridiron/controls/page.hpp>
#include <gridiron/controls/control.hpp>

namespace GridIron
{
    const std::string Control::HtmlNamespace = GRIDIRON_XHTML_NS;

    Control::Control(std::string id, std::shared_ptr<Control> parent)
    {
        std::shared_ptr<Control> result = nullptr;
        std::shared_ptr<Control> _Page;

        // INITIALIZE VARIABLES
        // our id
        _id = id;
        // our parent, as given
        _parent.swap(parent);
        // whether this page should be serialized into the viewstate
        _viewStateEnabled = false;
        // whether this is an autonomous control - affects behavior in derived classes
        _autonomous = false;
        // this is a pointer to the html Tag associated with this instance
        _htmlNode = nullptr;
        // TODO: check types against an allowedTypes virtual method?

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

    std::shared_ptr<Control> Control::This()
    {
        return shared_from_this();
    }

    // find the bottom-most control, regardless of type
    // returns: pointer - may be self
    std::shared_ptr<Control>
    Control::GetRoot(void)
    {
        std::shared_ptr<Control> ptr = This();

        while (ptr->_parent != nullptr)
        {
            return _parent->This();
        }

        return ptr;
    }

    // find the bottom-most control, only if a Page object
    // returns: pointer on success or nullptr, may be self
    std::shared_ptr<Control>
    Control::GetPage(void)
    {
        std::shared_ptr<Control> ptr = GetRoot();
        Control *p = ptr.get();
        if (!Page::instanceOf<Page>(p))
            return nullptr;
        else
            return ((Page *)p)->This();
    }

    std::shared_ptr<Control> Control::Find(Control &control)
    {
        auto it = _controlsByControl.find(&control);
        if (it != _controlsByControl.end())
        {
            return it->second;
        }
        return nullptr;
    }

    // recursive search through all controls under this object, return the one with specified id
    std::shared_ptr<Control> Control::FindByID(const std::string &id, bool searchParentsIfNotChild = false)
    {
        // Check immediate children first
        for (auto &child : _children)
        {
            if (child->ID() == id)
            {
                return child;
            }
        }

        // Optionally search in the global map
        if (searchParentsIfNotChild)
        {
            auto it = _controlsByID.find(id);
            if (it != _controlsByID.end())
            {
                return it->second;
            }
        }

        return nullptr;
    }

    static std::string GetFullName(std::string tag)
    {
        return (HtmlNamespace + "::" + tag);
    }

    std::string Control::fullName()
    {
        return GetFullName(this->controlTagName());
    }

    std::ostream &Control::fullName(std::ostream &os)
    {
        os << GetFullName(this->controlTagName());
        return os;
    }

    std::string Control::controlTagName() const
    {
        return "Control";
    }

    std::string Control::renderTagName() const
    {
        return "div";
    }

    // register this control with the parent
    bool Control::registerChild(std::string id, Control *control)
    {
        if (id.empty() || control == nullptr)
        {
            return false;
        }

        auto sharedControl = std::shared_ptr<Control>(control);
        _children.push_back(sharedControl);
        _controlsByID[id] = sharedControl;
        _controlsByControl[control] = sharedControl;

        return true;
    }

    // unregister this control from the parent
    bool Control::unregister_child(std::string &id)
    {
        auto it = std::find_if(_children.begin(), _children.end(),
                               [&id](const std::shared_ptr<Control> &control)
                               { return control->ID() == id; });
        if (it != _children.end())
        {
            _controlsByID.erase(id);
            _controlsByControl.erase(it->get());
            // Remove from other maps if necessary
            // ...

            _children.erase(it);
            return true;
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

    std::shared_ptr<Control> Control::fromHtmlNode(htmlnode &node)
    {
        // Ensure the node is valid
        if (node == nullptr)
        {
            throw GridException(301, "HTML Tag not found for this instance");
        }

        // Create a new Control instance
        std::shared_ptr<Control> control = std::make_shared<Control>();

        // Set the HTML node for the control
        control->SetHTMLNode(&node);

        // Extract the ID from the node, if available
        std::string id = ""; // Replace with actual method to extract ID from node
        control->_id = id;

        // Check if the control is part of a page
        std::shared_ptr<Page> _Page = control->GetPage();
        if (_Page == nullptr)
        {
            throw GridException(300, "Control must be attached to a page");
        }

        // Process the text content of the node
        // This is a placeholder for actual logic to process the node's content
        std::string textContent = ""; // Replace with actual method to extract text content
        control->_text = textContent;

        // Register the text string for access if the control is autonomous
        if (control->_autonomous)
        {
            _Page->RegisterVariable(id + ".Text", &control->_text);
        }

        return control;
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
                return _controlProxies->at(i)->CreateObject(id, parent, type);
            }
        }
        return nullptr;
    }

    // global factory instance
    ControlFactory globalControlFactory;
}
