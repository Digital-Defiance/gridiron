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
#include <gridiron/properties.hpp>
#include <gridiron/controls/ControlFactory.hpp>

namespace GridIron {
    namespace Controls{
        class Page;
        class ControlFactoryProxyBase;
        using namespace GridIron::Properties;

        // custom control base class, must derive
        class Control :
        public GridIron::Node {
            protected:
            Control(
            const char *id, Control
            *parent = nullptr);                // parent can be page type or control type
            public:
            inline static const std::string ControlType = "Control";
            friend class ControlFactoryProxyBase;
            friend class ContentMappedProperty;
            friend class AttributeMappedProperty;
            friend class ComparedAttributeProperty;

            ~Control();                                    // destructor
            class : public ReadOnlyProperty<std::string> { friend class Control; } ID;       // our id
            const Control *const Parent;                                                     // our parent
            inline static const Controls::ControlPass Pass = Controls::ControlPass::FIRST;   // which loop this control is interpreted on
            inline static const std::string Namespace = GRIDIRON_XHTML_NS;                   // gridiron namespace so it can be accessed as a regvar (needs pointed to string)
            inline static const std::string RenderTag = "div";    // the associated codebeside tag name eg <namespace>::<tag>
            inline static const bool IsPage = false;              // whether this control is a page
            inline static const bool AllowAutonomous = false;     // can't have an autonomous page class
            class : public ReadOnlyProperty<bool> {
                friend class Control;
            } Autonomous;      // control does not have a pre-programmed instance, instantiated from the HTML
            inline static const bool ViewStateEnabled = false;    // whether to bother serializing this object
            bool ViewStateValid = false;      // whether viewstate was authenticated
            class : public ReadOnlyProperty<Page *> {
                friend class Control;
            } RootPage;       // what page this control belongs to

            Control *
            FindByID(const std::string &id);                    // find by id, starting with the current instance
            std::ostream &NamespacedTag(std::ostream &os);

            const std::string NamespacedTag();

            friend std::ostream &operator<<(std::ostream &os, const Control &control);

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
            void EnableAutonomous(); // set whether the control is in html only (no C++ instance pre-programmed)

            virtual bool
            registerChild(const std::string id,
                          Control &control);    // add child control's id and name to the bimap
            virtual bool
            unregister_child(
                    const std::string id);                    // delete child control's id and name from the bimap

            std::string _parsed;                                               // data after parsing- only data relevant to our id

            /* These vars correspond to whether (and where) the C++ instance has been matched to an HTML instance (and only one)
             * Multiple detections of HTML tags with the same ID should cause an error, regardless of type
             * Similarly, the page should only be able to contain one control of a given ID
             */

            // memory overhead warning...
            static std::map<const std::string, Control *> _controlsByID;
            static std::map<const std::string, std::vector<Control *>> _controlsByNamespace;
            static std::map<const std::string, std::vector<Control *>> _controlsByControlTag;
            static std::set<std::string> _registeredNamespaces;
        };
    };
};

#endif
