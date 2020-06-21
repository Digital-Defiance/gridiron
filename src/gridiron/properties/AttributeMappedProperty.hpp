#ifndef GRIDIRON_ATTRIBUTEMAPPEDPROPERTY_HPP
#define GRIDIRON_ATTRIBUTEMAPPEDPROPERTY_HPP

#include <string>
#include <gridiron/properties.hpp>
#include <gridiron/Node.hpp>
#include <gridiron/properties/ComparedAttributeProperty.hpp>

namespace GridIron {
    namespace Properties {
        /**
         * Maps a nodes attribute onto a property for both read and write
         * writes to the property update the associated attribute
         */
        class AttributeMappedProperty : public Property<std::string> {
        public:
            friend class GridIron::Node;

            AttributeMappedProperty(Node *n, const std::string& attribute) :
                    attachedNode{n},
                    attribute_{attribute},
                    originalData{n->getAttribute(attribute)},
                    Changed{n, n->getAttribute(attribute)} {}

            AttributeMappedProperty(AttributeMappedProperty &property) :
                    attachedNode{property.attachedNode},
                    attribute_{property.attribute_},
                    originalData{property.originalData},
                    Changed{property.attachedNode, property.attribute_} {}

            // access with function call syntax
            inline const std::string& operator()() const {
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
            inline const std::string& get() const {
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

            inline const std::string& operator=(std::string const &value) {
                return set(value);
            }

            ComparedAttributeProperty Changed;

        protected:
            GridIron::Node *attachedNode;
            const std::string attribute_;
            const std::string originalData;
        };
    }
}
#endif //GRIDIRON_ATTRIBUTEMAPPEDPROPERTY_HPP
