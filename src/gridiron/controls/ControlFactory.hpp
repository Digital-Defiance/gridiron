#ifndef GRIDIRON_CONTROLFACTORY_HPP
#define GRIDIRON_CONTROLFACTORY_HPP

namespace GridIron {
    namespace Controls {
        class Control;
        class ControlFactoryProxyBase;

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

            Control* CreateByType(const char *type, const char *id, Control* parent);

            int GetCount();

            const ControlFactoryProxyBase *GetAt(int i);

        private:
            void Init();

            factory_vector *_controlProxies;
        };

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
                return pointer;
            }
            // Member functions in T are static
            virtual const char* GetType() const
            { return T::GetType(); }

        };

        // global instance created in control.cpp
        ControlFactory globalControlFactory;
    };
};
#endif //GRIDIRON_CONTROLFACTORY_HPP
