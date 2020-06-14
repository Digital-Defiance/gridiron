#include <gridiron/gridiron.hpp>

namespace GridIron {
    Value::Value(std::shared_ptr<Control> parent, std::string key, std::string value) :
        Control(std::string("value-").append(key).c_str(), parent),
        This{shared_from_this()},
        key{key},
        value{value}
    {
        this->tagName_ = std::string(GRIDIRON_XHTML_NS).append("::Value");
    }

    ////////////////////////////////////////////////////////////
    // Declare an instance of the proxy to register the
    // existence of Label with the ControlFactory
    // !! only do this for classes that support autos !!
    static GridIron::ControlFactoryProxy<GridIron::Value> globaValueProxy;
}