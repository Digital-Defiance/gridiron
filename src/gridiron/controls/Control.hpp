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

#ifndef _CONTROL_HPP_
#define _CONTROL_HPP_

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <memory>
#include <gridiron/exceptions.hpp>
#include <gridiron/Node.hpp>
#include <gridiron/controls/ControlPass.hpp>
#include <gridiron/ParserDom.hpp>

namespace GridIron {
    class Page;
    class ControlFactoryProxyBase;

    // custom control base class, must derive
    class Control : public GridIron::Node {
    protected:
        Control(const char *id, Control* parent = nullptr);                // parent can be page type or control type
    public:
        ROProperty<std::string> ID;                                        // our id
        ROProperty<Controls::ControlPass> Pass = Controls::ControlPass::FIRST;                 // which pass the control is expected to be rendered on
        ROProperty<const char *> Namespace = GRIDIRON_XHTML_NS;            // gridiron namespace so it can be accessed as a regvar (needs pointed to string)
        ROProperty<const char *> RenderTag = "html";                       // the associated codebeside tag name eg <namespace>::<tag>
        ROProperty<bool> IsPage = false;
        ROProperty<bool> Autonomous = ROProperty<bool>(false);          // control does not have a pre-programmed instance, instantiated from the HTML
        ROProperty<bool> AllowAutonomous = false;                          // can't have an autonomous page class
        ROProperty<bool> ViewStateEnabled = ROProperty<bool>(false);    // whether to bother serializing this object
        ROProperty<bool> ViewStateValid = ROProperty<bool>(false);      // whether viewstate was authenticated

        ~Control();                                    // destructor
        Page*
        GetPage(ParserDom &parser, bool rootOnly = false);                                        // return pointer to parent page object (or self for page)
        Control*
        GetRoot(ParserDom &parser);                                    // return pointer to the parent control object, regardless of type.

        Control* FindByID(const std::string &id);                    // find by id, starting with the current instance
        std::ostream &NamespacedTag(std::ostream &os);

        std::string NamespacedTag();

        friend std::ostream &operator<<(std::ostream &os, const Control &control);

        void EnableAutonomous();                                    // set whether the control is in html only (no C++ instance pre-programmed)

        template<typename Base, typename T>
        static inline bool instanceOf(const T *ptr) {
            return dynamic_cast<const Base *>(ptr) != nullptr;
        }

        template<typename Base, typename T>
        static inline bool instanceOf(const std::shared_ptr<T> ptr) {
            return dynamic_cast<const Base *>(ptr.get()) != nullptr;
        }

//        static std::shared_ptr<Control> fromHtmlNode(htmlcxx2::HTML::Node &node);

    protected:
        virtual bool
        registerChild(const std::string id, Control &control);    // add child control's id and name to the bimap
        virtual bool
        unregister_child(const std::string id);                    // delete child control's id and name from the bimap

        std::string _parsed;                                               // data after parsing- only data relevant to our id

        /* These vars correspond to whether (and where) the C++ instance has been matched to an HTML instance (and only one)
         * Multiple detections of HTML tags with the same ID should cause an error, regardless of type
         * Similarly, the page should only be able to contain one control of a given ID
         */

        // memory overhead warning...
        static std::map<const std::string, Control*> _controlsByID;
        static std::map<const std::string, std::vector<Control*>> _controlsByNamespace;
        static std::map<const std::string, std::vector<Control*>> _controlsByControlTag;
        static std::set<std::string> _registeredNamespaces;
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

        std::shared_ptr<Control> CreateByType(const char *type, const char *id, Control* parent);

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
    class ControlFactoryProxyBase {
    public:
        inline ControlFactoryProxyBase() {
            globalControlFactory.Register(this);
        }

        virtual Control* CreateObject(const char *id, Control* parent) const = 0;

        // Expose criteria here
        virtual const char* GetType() const = 0;
    };

    // instantiate one of these in the .cpp file of every derived control class you want autos for
    template<class T>
    class ControlFactoryProxy : public ControlFactoryProxyBase {
        inline virtual Control* CreateObject(const char *id, Control* parent) const {
            if (!T::AllowAutonomous) return nullptr;
            Control* pointer = new T(id, parent);
            if (pointer == nullptr) return nullptr;
            pointer->Control::EnableAutonomous();
            return pointer;
        }
        // Member functions in T are static
        virtual const char* GetType() const
        { return T::GetType(); }

    };
}

#endif
