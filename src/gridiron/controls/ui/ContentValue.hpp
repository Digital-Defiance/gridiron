#ifndef GRIDIRON_CONTENTVALUE_HPP
#define GRIDIRON_CONTENTVALUE_HPP

#include <gridiron/controls/Control.hpp>
#include <gridiron/properties/AttributeMappedProperty.hpp>
#include <gridiron/properties/ContentMappedProperty.hpp>

namespace GridIron {
    namespace Controls {
        namespace UI {
            class Control;

            class ContentValue : public GridIron::Controls::Control {
            public:
                inline static const std::string ControlType = "ContentValue";
                ContentValue(Control* parent, const std::string &key, std::string value);

                inline static const Controls::ControlPass Pass = Controls::ControlPass::SECOND;   // which loop this control is interpreted on

                AttributeMappedProperty key;
                ContentMappedProperty value;
                inline static const bool AllowAutonomous = true;
            protected:
            };
        }
    }
}
#endif //GRIDIRON_CONTENTVALUE_HPP
