#include <gridiron/controls/ui/ContentValue.hpp>

namespace GridIron {
    namespace Controls {
        namespace UI {
            ContentValue::ContentValue(Control* parent, const std::string &key, std::string value) :
                Control(std::string("value-").append(key).c_str(), parent)
            {
                key = AttributeMappedProperty((Node &) this, "key");
            }
        }
    }

    ////////////////////////////////////////////////////////////
    // Declare an instance of the proxy to register the
    // existence of Label with the ControlFactory
    // !! only do this for classes that support autos !!
    static GridIron::Controls::ControlFactoryProxy<GridIron::Controls::UI::ContentValue> globalContentValueProxy;
}