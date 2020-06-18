#ifndef GRIDIRON_CONTENTMAPPEDPROPERTY_HPP
#define GRIDIRON_CONTENTMAPPEDPROPERTY_HPP

#include <string>
#include <gridiron/properties.hpp>
#include <gridiron/Node.hpp>
#include <gridiron/ComparedAttributeProperty.hpp>

namespace GridIron {
    /**
     * Maps a nodes content onto a property for both read and write
     * writes to the property update the associated node content
     * NOTE: Only one ContentMappedProperty per node/control has any meaning
     */
    class ContentMappedProperty : public BaseProperty<std::string> {
    public:
    public:
        // access with function call syntax
        ContentMappedProperty(std::shared_ptr<GridIron::Node> node) :
            attachedNode{node},
            original{node->text()},
            Changed{node, node->text()} {}

        ContentMappedProperty(ContentMappedProperty &property) :
            attachedNode{property.attachedNode},
            original{property.original},
            Changed{property.attachedNode, property.original} {}

        inline std::string operator()() const {
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
        inline std::string get() const {
            return attachedNode->text();
        }

        inline std::string set(const std::string &value) {
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

        inline std::string operator=(std::string const &value) {
            return set(value);
        }

        ComparedAttributeProperty Changed;

    protected:
        std::shared_ptr <GridIron::Node> attachedNode;
        std::string original;
    };
}
#endif //GRIDIRON_CONTENTMAPPEDPROPERTY_HPP
