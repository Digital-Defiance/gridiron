#include <gridiron/controls/ui/AttributeValue.hpp>

namespace GridIron {
    namespace Controls {
        namespace UI {
            AttributeValue::AttributeValue(std::shared_ptr<Control> parent, const std::string &key, std::string value) :
                    Control(std::string("value-").append(key).c_str(), parent),
                    key{This(), key},
                    value{This(), value}
            {
                this->tagName_ = std::string(GRIDIRON_XHTML_NS).append("::AttributeValue");
            }

            ////////////////////////////////////////////////////////////
            // Declare an instance of the proxy to register the
            // existence of Label with the ControlFactory
            // !! only do this for classes that support autos !!
            static GridIron::ControlFactoryProxy<GridIron::Controls::UI::AttributeValue> globalAttributeValueProxy;
        }
    }
}