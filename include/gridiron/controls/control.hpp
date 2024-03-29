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

#ifndef _CONTROL_HPP_
#define _CONTROL_HPP_

#include <string>
#include <iostream>
#include <fstream>
#include <gridiron/exceptions.hpp>
#include <sstream>
#include <vector>
#include <map>
#include <memory>

namespace GridIron
{
    class Page;

    class Control;

    class ControlFactoryProxyBase;

    // map between control id's and control instances
    typedef std::map<std::string, std::shared_ptr<Control>> control_map;
    typedef std::vector<std::shared_ptr<Control>> vector_control_children;

    // custom control base class, must derive
    class Control : public std::enable_shared_from_this<Control>
    {
    protected:
        Control(std::string id, std::shared_ptr<Control> parent); // parent can be page type or control type
    public:
        static const std::string HtmlNamespace; // gridiron namespace so it can be accessed as a regvar (needs pointed to string)

        static std::string GetFullName(std::string tag);

        virtual ~Control(); // destructor
        std::shared_ptr<Control>
        GetPage(); // return pointer to parent page object (or self for page)
        std::shared_ptr<Control>
        GetRoot(); // return pointer to the parent control object, regardless of type.
        std::shared_ptr<Control> This();

        std::shared_ptr<Control> Find(Control &control);

        std::shared_ptr<Control> FindByID(const std::string &id,
                                          bool searchParentsIfNotChild = false); // find by id, starting with the current instance
        std::ostream &fullName(std::ostream &os);

        std::string fullName();

        friend std::ostream &operator<<(std::ostream &os, const Control &control);

        void SetHTMLNode(htmlnode *node);

        inline bool HTMLNodeRegistered() { return (this->_htmlNode != NULL); };

        void SetAutonomous(
            bool isauto); // set whether the control is in html only (no C++ instance pre-programmed)
        inline virtual bool IsAutonomous(
            bool isauto) { return this->_autonomous; };      // whether the control is in html only (no C++ instance pre-programmed)
        inline const std::string ID() { return this->_id; }; // return our ID

        template <typename Base, typename T>
        static inline bool instanceOf(const T *ptr)
        {
            return dynamic_cast<const Base *>(ptr) != nullptr;
        }

        virtual std::string controlTagName() const = 0; // the associated codebeside tag name eg <namespace>::<tag>
        virtual std::string renderTagName() const = 0;  // the associated html tag name eg <div>

        static std::shared_ptr<Control> fromHtmlNode(htmlnode &node);

    protected:
        inline static const bool AllowAutonomous() { return false; } // can't have a base class anyway
        virtual bool
        registerChild(std::string id, Control *control); // add child control's id and name to the bimap
        virtual bool
        unregister_child(std::string &id); // delete child control's id and name from the bimap

        std::string _id;                   // our id
        vector_control_children _children; // collection of pointers to child controls, by their address
        std::shared_ptr<Control> _parent;  // parent's pointer
        std::string _parsed;               // data after parsing- only data relevant to our id
        bool _viewStateEnabled = false;    // whether to bother serializing this object
        bool _viewStateValid = false;      // whether viewstate was authenticated
        bool _autonomous = false;          // control does not have a pre-programmed instance, instantiated from the HTML

        /* These vars correspond to whether (and where) the C++ instance has been matched to an HTML instance (and only one)
         * Multiple detections of HTML tags with the same ID should cause an error, regardless of type
         * Similarly, the page should only be able to contain one control of a given ID
         */
        htmlnode *_htmlNode; // the associated html node
        std::string _text;

        // memory overhead warning...
        static std::map<std::string, std::shared_ptr<Control>> _controlsByID;
        static std::map<Control *, std::shared_ptr<Control>> _controlsByControl;
        static std::map<std::string, std::shared_ptr<Control>> _controlsByNamespace;
        static std::map<std::string, std::shared_ptr<Control>> _controlsByControlTag;
        static std::set<std::string> _registeredNamespaces;
    };

    // --------------------------------------------------------------------
    // Thank you Dr. Dobbs - August 01, 1998 (http://www.ddj.com/184410633)
    // See control.cpp for comments specific to this implementation
    // NOTE: the order of declaration here is very sensitive

    typedef std::vector<const ControlFactoryProxyBase *> factory_vector;

    // definition for our class in charge of registering, finding, and instantiating control classes
    // by their type
    class ControlFactory
    {
    public:
        ControlFactory();

        ~ControlFactory();

        void Register(const ControlFactoryProxyBase *proxy);

        Control *CreateByType(const char *type, const char *id, Control *parent);

        int GetCount();

        const ControlFactoryProxyBase *GetAt(int i);

    private:
        void Init();

        factory_vector *_controlProxies;
    };

    // global instance created in control.cpp
    extern ControlFactory globalControlFactory;

    // each proxy, when instantiated by the derived control class will register it self with the
    // control factory. This will be derived by the template class that follows.
    class ControlFactoryProxyBase
    {
    public:
        inline ControlFactoryProxyBase()
        {
            globalControlFactory.Register(this);
        }

        virtual Control *CreateObject(const char *id, Control *parent, std::string type) const = 0;
        virtual const char *GetType() const = 0;
    };

    // instantiate one of these in the .cpp file of every derived control class you want autos for
    template <class T>
    class ControlFactoryProxy : public ControlFactoryProxyBase
    {
        inline virtual Control *CreateObject(const char *id, Control *parent, std::string type) const
        {
            if (!T::AllowAutonomous())
                return NULL;
            Control *pointer = new T(id, parent, type);
            if (pointer == NULL)
                return NULL;
            pointer->Control::SetAutonomous(true);
            return pointer;
        }
        inline virtual const char *GetType() const
        {
            return T::Type();
        }
    };
}

#endif
