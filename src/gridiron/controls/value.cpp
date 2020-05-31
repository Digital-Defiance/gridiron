#include <gridiron/controls/value.hpp>

namespace GridIron {
    static ROProperty<std::string> Value::Namespace = GRIDIRON_XHTML_NS;
    static ROProperty<std::string> Value::Tag = "Value";

    Value::Value(std::shared_ptr<Control> parent, std::string key, std::string value = "") :
        This{std::shared_from_this()},
        tagName_{"div"},
        _key{key},
        value{value} {}
}