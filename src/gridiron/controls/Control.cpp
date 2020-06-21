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

#include <gridiron/controls/Control.hpp>
#include <gridiron/ParserDom.hpp>

namespace GridIron {

    namespace Controls {
        class Page;

        Control::Control(const char *id, Control *parent) : Parent{parent} {
            // we must have an ID- and only one instance of an id may exist on all controls under a page object
            if (strlen(id) == 0) throw GridException(200, "no id specified");

            // set up readonly attrs
            ID.value = std::string(id);
            tagName_ = NamespacedTag();
            Autonomous.value = false;

            // find the page control if we have one, then look to see if the id is already registered
            const Control *ptr = parent;
            while (ptr != nullptr) {
                if (ptr->IsPage) {
                    RootPage.value = (Page *) ptr;
                    break;
                } else {
                    // another control
                    ptr = ptr->Parent;
                }
            }

            if (RootPage.operator->() != nullptr) {
                // check page for existing controls with that id
                Control *result = reinterpret_cast<Control *>(RootPage.value)->FindByID(ID.value);
                if (result != nullptr) throw GridException(201, "id already in use");
            }
        }

        std::ostream &operator<<(std::ostream &os, const Control &control) {
            os << control;
            return os;
        }

        // recursive search through all controls under this object, return the one with specified id
        Control *
        Control::FindByID(const std::string &id) {
            std::map<const std::string, Control *>::iterator it = _controlsByID.find(id);
            if (it != _controlsByID.end()) {
                return it->second;
            }

            // no luck
            return nullptr;
        }

        std::ostream &Control::NamespacedTag(std::ostream &os) {
            os << this->Namespace << "::" << this->ControlType;
            return os;
        }

        const std::string Control::NamespacedTag() {
            return (this->Namespace + "::" + this->ControlType);
        }

        // destructor
        Control::~Control() {}

        // friend class Page?
        // allow the page class to tell us we're an anonymous instance
        void
        Control::EnableAutonomous() {
            if (!this->AllowAutonomous) {
                throw GridException(500, "AllowAutonomous is not enabled");
            }
            this->Autonomous.value = true;
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
        Control *
        ControlFactory::CreateByType(const char *type, const char *id, Control *parent) {
            std::cerr << "Requesting creation of type " << type << std::endl;
            for (int i = 0; i < _controlProxies->size(); ++i) {
                if (strcmp(_controlProxies->at(i)->GetType(), type) == 0) {
                    return _controlProxies->at(i)->CreateObject(id, parent);
                }
            }
            return nullptr;
        }

        // global factory instance
        extern ControlFactory globalControlFactory;
    }
}

