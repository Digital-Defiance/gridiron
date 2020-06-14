#ifndef GRIDIRON_VALUE_HPP
#define GRIDIRON_VALUE_HPP

#define GRIDIRON_NESTED_EXCLUDE_VALUE
#include <gridiron/gridiron.hpp>

namespace GridIron {
    class Control;

    class Value : public Control
    {
    public:
        ROProperty<ControlPass> Pass = ControlPass::FIRST;         // which pass the control is expected to be rendered on
        ROProperty<const char *> Namespace = GRIDIRON_XHTML_NS;    // gridiron namespace so it can be accessed as a regvar (needs pointed to string)
        ROProperty<const char *> RenderTag = "div";    // the associated codebeside tag name eg <namespace>::<tag>

        ROProperty<std::shared_ptr<Control>, std::shared_ptr<Value>> This;
        Value(std::shared_ptr<Control> parent, const std::string key, std::string value = "");
        const std::string key;
        RWProperty<std::string> value;
        ROProperty<bool> AllowAutonomous = true;
    protected:
    };
}
#endif //GRIDIRON_VALUE_HPP
