#ifndef GRIDIRON_CONTENTMAPPEDPROPERTY_HPP
#define GRIDIRON_CONTENTMAPPEDPROPERTY_HPP

#include <string>
#include <gridiron/Node.hpp>
#include <gridiron/properties/Property.hpp>
#include <gridiron/properties/ComparedAttributeProperty.hpp>

namespace GridIron {
    namespace Controls {
        class Control;
    }
    namespace Properties {
        /**
         * Maps a nodes content onto a property for both read and write
         * writes to the property update the associated node content
         * NOTE: Only one ContentMappedProperty per node/control has any meaning
         */
        class ContentMappedProperty : public Property<std::string> {
        public:
            // access with function call syntax
            ContentMappedProperty(GridIron::Node *n) :
                attachedNode{n},
                original{std::string(n->text_)},
                Changed{this->original, n->text_} {};

            ContentMappedProperty(ContentMappedProperty &property) :
                    attachedNode{property.attachedNode},
                    original{property.original},
                    Changed{this->original, property.attachedNode->text_} {}

            inline const std::string& operator()() const {
                return this->get();
            }

            inline std::string operator()(const std::string &value) {
                return set(value);
            }

            friend inline std::ostream &operator<<(std::ostream &os, ContentMappedProperty &property) {
                os << property.get();
                return os;
            }

            // access with get()/set() syntax
            inline std::string& get() const {
                return attachedNode->text_;
            }

            inline const std::string& set(const std::string &value) {
                attachedNode->updateText(value);
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

            CompareProperty<std::string> Changed;

        protected:
            GridIron::Node *attachedNode;
            std::string original;
        };
    }
}
#endif //GRIDIRON_CONTENTMAPPEDPROPERTY_HPP
