#ifndef GRIDIRON_ATTRIBUTEVALUE_HPP
#define GRIDIRON_ATTRIBUTEVALUE_HPP

#include <gridiron/controls/Control.hpp>
#include <gridiron/properties/AttributeMappedProperty.hpp>

namespace GridIron {
    namespace Controls {
        class Control;
        namespace UI {
        class AttributeValue : public GridIron::Controls::Control
            {
            public:
                AttributeValue(std::shared_ptr<Control> parent, const std::string &key, std::string value);
                inline static const Controls::ControlPass Pass = Controls::ControlPass::SECOND; // which pass the control is expected to be rendered on
                AttributeMappedProperty key;
                AttributeMappedProperty value;
                inline static const bool AllowAutonomous = true;
            protected:
            };
        }
    }
}
#endif //GRIDIRON_ATTRIBUTEVALUE_HPP
