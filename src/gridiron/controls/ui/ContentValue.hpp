#ifndef GRIDIRON_CONTENTVALUE_HPP
#define GRIDIRON_CONTENTVALUE_HPP

#include <gridiron/controls/Control.hpp>

namespace GridIron {
    namespace Controls {
        namespace UI {
            class Control;

            class ContentValue : public GridIron::Control {
            public:
                ContentValue(std::shared_ptr<Control> parent, const std::string key, std::string value = "");

                ROProperty<ControlPass> Pass = ControlPass::SECOND;         // which pass the control is expected to be rendered on
                ROProperty<const char *> Namespace = GRIDIRON_XHTML_NS;    // gridiron namespace so it can be accessed as a regvar (needs pointed to string)
                ROProperty<const char *> RenderTag = "div";    // the associated codebeside tag name eg <namespace>::<tag>

                ROProperty<std::shared_ptr<Control>, std::shared_ptr<ContentValue>> This;
                AttributeMappedProperty<std::string> key;
                ContentMappedProperty<std::string> value;
                ROProperty<bool> AllowAutonomous = true;
            protected:
            };
        }
    }
}
#endif //GRIDIRON_CONTENTVALUE_HPP
