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

#ifndef _CONTROL_HPP_
#define _CONTROL_HPP_

#include <gridiron/base_classes/exceptions.hpp>
#include <gridiron/base_classes/page.hpp>
// standard
#include <string>

namespace GridIron {
    class Page;

    class Control;

    class ControlFactoryProxyBase;

    // map between control id's and control instances
    typedef std::map<std::string *, Control *> child_map;

    // custom control base class, must derive
    class Control {
    public:
        Control(const char *id, Control *parent);                // parent can be page type or control type
        virtual ~Control();                                    // destructor
        Page *
        GetPage();                                        // return pointer to parent page object (or self for page)
        Control *
        GetRoot();                                    // return pointer to the parent control object, regardless of type.
        Control *Find(std::string const &id);                    // find by id, starting with the current instance
        bool Is(std::string type);                            // return whether the classes type name matches
        virtual std::string render() = 0;                        // render the html for this and all contained children (pure virtual abstract)
        void SetHTMLNode(htmlnode *node);

        inline bool HTMLNodeRegistered() { return (this->_html_node != NULL); };

        void SetAutonomous(
                bool isauto);                                    // set whether the control is in html only (no C++ instance pre-programmed)
        inline virtual bool IsAutonomous(
                bool isauto) { return this->_autonomous; };    // whether the control is in html only (no C++ instance pre-programmed)
        inline const char *
        Type() { return this->_classtype.c_str(); };    // whether the control is in html only (no C++ instance pre-programmed)
        inline const std::string ID() { return this->_id; };                // return our ID

    protected:
        inline static const bool AllowAutonomous() { return false; }        // can't have a base class anyway
        inline static const char *ClassType() { return "Control"; };        // return our type
        virtual void
        parse() = 0;                                            // parse the parent's data looking for our tags (pure virtual abstract)
        virtual bool
        register_child(std::string *id, Control *pointer);    // add child control's id and name to the bimap
        virtual bool
        unregister_child(std::string &id);                    // delete child control's id and name from the bimap

        std::string _id;                            // our id
        child_map _children;                        // collection of pointers to child controls, by their address
        Control *_parent;                            // parent's pointer
        std::string _parsed;                        // data after parsing- only data relevant to our id
        bool _enableviewstate;                    // whether to bother serializing this object
        bool _autonomous;                            // control does not have a pre-programmed instance, instantiated from the HTML
        std::string _classtype;

        /* These vars correspond to whether (and where) the C++ instance has been matched to an HTML instance (and only one)
         * Multiple detections of HTML tags with the same ID should cause an error, regardless of type
         * Similarly, the page should only be able to contain one control of a given ID
         */
        htmlnode *_html_node;                    // the associated html node
    };

    // --------------------------------------------------------------------
    // Thank you Dr. Dobbs - August 01, 1998 (http://www.ddj.com/184410633)
    // See control.cpp for comments specific to this implementation
    // NOTE: the order of declaration here is very sensitive

    typedef std::vector<const ControlFactoryProxyBase *> factory_vector;

    // definition for our class in charge of registering, finding, and instantiating control classes
    // by their type
    class ControlFactory {
    public:
        ControlFactory();

        ~ControlFactory();

        void Register(const ControlFactoryProxyBase *proxy);

        Control *CreateByType(const char *type, const char *id, Control *parent);

        int GetCount();

        const ControlFactoryProxyBase *GetAt(int i);

    private:
        void Init();

        factory_vector *_controlproxies;
    };

    // global instance created in control.cpp
    extern ControlFactory g_controlfactory;

    // each proxy, when instantiated by the derived control class will register it self with the
    // control factory. This will be derived by the template class that follows.
    class ControlFactoryProxyBase {
    public:
        inline ControlFactoryProxyBase() {
            g_controlfactory.Register(this);
        }

        virtual Control *CreateObject(const char *id, Control *parent) const = 0;

        virtual const char *GetType() const = 0;
    };

    // instantiate one of these in the .cpp file of every derived control class you want autos for
    template<class T>
    class ControlFactoryProxy : public ControlFactoryProxyBase {
        inline virtual Control *CreateObject(const char *id, Control *parent) const {
            if (!T::AllowAutonomous()) return NULL;
            Control *pointer = new T(id, parent);
            if (pointer == NULL) return NULL;
            pointer->Control::SetAutonomous(true);
            return pointer;
        }

        // Member functions in T are static
        inline virtual const char *GetType() const { return T::ClassType(); }
    };
}

#endif
