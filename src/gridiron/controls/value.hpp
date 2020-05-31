#ifndef GRIDIRON_VALUE_HPP
#define GRIDIRON_VALUE_HPP

#include <gridiron/gridiron.hpp>

namespace GridIron {
    class Control;

    class Value : public Control {
    public:
        ROProperty<std::shared_ptr<Value>> This;
        Value(std::shared_ptr<Control> parent, const std::string key, std::string value = "");
        const std::string key;
        RWProperty<std::string> value;
    protected:
    };
}
#endif //GRIDIRON_VALUE_HPP
