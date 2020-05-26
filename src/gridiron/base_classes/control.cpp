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
 * CGICC is under GPLv3
 * Boost is under the Boost license
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
#include <iostream>

namespace GridIron {

    Control::Control(const char *id, Control *parent) {
        Control * result = NULL;
        Page *_Page;

        // INITIALIZE VARIABLES
        // our id
        _id = std::string(id);
        // our parent, as given
        _parent = parent;
        // whether this page should be serialized into the viewstate
        _enableviewstate = false;
        // whether this is an autonomous control - affects behavior in derived classes
        _autonomous = false;
        // this is to make the class type accessible from the base class
        _classtype = Control::ClassType();
        // this is a pointer to the html tag associated with this instance
        _html_node = NULL;

        // we must have an ID- and only one instance of an id may exist on all controls under a page object
        if (_id.length() == 0) throw GridException(200, "no id specified");

        // find the page control if we have one, then look to see if the id is already registered
        _Page = GetPage();
        if (_Page != NULL) {
            // check page for existing controls with that id
            result = _Page->Find(id);
            if (result != NULL) throw GridException(201, "id already in use");

            // register ourselves with the parent if we have one (pages dont)
            // parent will have a pointer to our id string to save mem and allow for changes
            if (_parent != NULL) _parent->register_child(&_id, this);
        }
    }

    // find the bottom-most control, regardless of type
    // returns: pointer - may be self
    Control *
    Control::GetRoot(void) {
        Control *ptr = this;

        while (ptr->_parent != NULL) {
            ptr = ptr->_parent;
        }
        return (Page *) ptr;
    }

    // fine the bottom-most control, only if a Page object
    // returns: pointer on success or NULL, may be self
    Page *
    Control::GetPage(void) {
        Control *ptr = GetRoot();
        if (strcmp(ptr->Type(), "Page") != 0) return NULL;
        else return (Page *) ptr;
    }

    // recursive search through all controls under this object, return the one with specified id
    Control *
    Control::Find(std::string const &id) {
        Control *ptr = NULL;

        if (_children.size() == 0) return NULL;

        // search child map recursively
        for (child_map::iterator i = _children.begin(), iend = _children.end(); i != iend; ++i) {
            // check the child's id first
            if (*(i->first) == id) {
                return (i->second);
            }
            // check its children, if any
            if ((i->second)->_children.size() > 0) {
                if (i->second == this) continue;    // avoid infinite loops
                ptr = (i->second)->Find(id);
                if (ptr != NULL) return ptr;
            }
        }

        // no luck
        return NULL;
    }

    // return whether this control is of 'type', mostly a timesaver
    bool
    Control::Is(std::string type) {
        return (strcmp(this->Type(), type.c_str()) == 0);
    }

    // register this control with the parent
    bool
    Control::register_child(std::string *id, Control *const pointer) {
        if ((id == NULL) || (pointer == NULL)) return false;

        // check for duplicates first
        for (child_map::iterator i = _children.begin(), iend = _children.end(); i != iend; ++i) {
            if (*(i->first) == *id) return false;    // id already in list
        }


        // then add
        _children[id] = pointer;
        std::cerr << "There are now " << _children.size() << " children registered." << std::endl;

        return true;
    }

    // unregister this control from the parent
    bool
    Control::unregister_child(std::string &id) {
        for (child_map::iterator i = _children.begin(), iend = _children.end(); i != iend; ++i) {
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
        if (_parent != NULL) _parent->unregister_child(_id);
    }

    // allow page class to tell us where our data is
    void
    Control::SetHTMLNode(htmlnode *node) {
        _html_node = node;
    }

    // allow the page class to tell us we're an anonymous instance
    void
    Control::SetAutonomous(bool isauto) {
        _autonomous = isauto;
    }

    // ------------------------------------------------
    // The following are used to allow us to instantiate control classes by type name as autonomous controls
    // Derived from Dr. Dobbs http://www.ddj.com/184410633
    ControlFactory::ControlFactory() {
        // MUST be empty- see article
    }

    ControlFactory::~ControlFactory() {
        // individual entries need not be deleted
        delete _controlproxies;
    }

    // if the list of proxies is empty, create it
    void
    ControlFactory::Init() {
        if (!_controlproxies) _controlproxies = new factory_vector;
    }

    // the derived control classes' .cpp files instantiate
    // a proxy class, which calls this, effectively adding themselves
    void
    ControlFactory::Register(const ControlFactoryProxyBase *proxy) {
        if (proxy == NULL) return;
        Init();
        _controlproxies->push_back(proxy);
    }

    // how many class types are registered
    int
    ControlFactory::GetCount() {
        if (!_controlproxies) return 0;
        else return _controlproxies->size();
    }

    // wrapper to get access
    const
    ControlFactoryProxyBase *
    ControlFactory::GetAt(int i) {
        return _controlproxies->at(i);
    }

    // iterates through the control types that have registered, tells the proxy to create us one if its type matches
    Control *
    ControlFactory::CreateByType(const char *type, const char *id, Control *parent) {
        std::cerr << "Requesting creation of type " << type << std::endl;
        for (int i = 0; i < _controlproxies->size(); ++i) {
            if (strcmp(_controlproxies->at(i)->GetType(), type) == 0) {
                return _controlproxies->at(i)->CreateObject(id, parent);
            }
        }
        return NULL;
    }

    // global factory instance
    ControlFactory g_controlfactory;
}

