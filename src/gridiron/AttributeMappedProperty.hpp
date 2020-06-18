#ifndef GRIDIRON_ATTRIBUTEMAPPEDPROPERTY_HPP
#define GRIDIRON_ATTRIBUTEMAPPEDPROPERTY_HPP

#include <string>
#include <gridiron/properties.hpp>
#include <gridiron/Node.hpp>
#include <gridiron/ComparedAttributeProperty.hpp>

namespace GridIron {
    /**
     * Maps a nodes attribute onto a property for both read and write
     * writes to the property update the associated attribute
     */
    class AttributeMappedProperty : public BaseProperty<std::string> {
    public:
        // access with function call syntax
        AttributeMappedProperty(std::shared_ptr<GridIron::Node> node, const std::string attribute) :
                attachedNode{node},
                attribute_{attribute},
                Changed{node, attribute} {}

        AttributeMappedProperty(AttributeMappedProperty &property) :
                attachedNode{property.attachedNode},
                attribute_{property.attribute_},
                Changed{property.attachedNode, property.attribute_} {}

        inline std::string operator()() const {
            return this->get();
        }

        inline std::string operator()(const std::string &value) {
            return set(value);
        }

        friend inline std::ostream &operator<<(std::ostream &os, AttributeMappedProperty &property) {
            os << property.get();
            return os;
        }

        // access with get()/set() syntax
        inline std::string get() const {
            if (attachedNode->hasAttribute(attribute_)) {
                std::string value;
                attachedNode->attribute(attribute_, value);
                return value;
            }
            return std::string();
        }

        inline std::string set(const std::string &value) {
            attachedNode->updateAttribute(attribute_, value);
            return value;
        }

        // access with '=' sign
        // in an industrial-strength library,
        // specializations for appropriate types might choose to
        // add combined operators like +=, etc.
        inline operator std::string() const {
            return get();
        }

        inline std::string operator=(std::string const &value) {
            return set(value);
        }

        ComparedAttributeProperty Changed;

    protected:
        std::shared_ptr<GridIron::Node> attachedNode;
        const std::string attribute_;
    };
}
#endif //GRIDIRON_ATTRIBUTEMAPPEDPROPERTY_HPP
