#ifndef GRIDIRON_COMPAREDATTRIBUTEPROPERTY_HPP
#define GRIDIRON_COMPAREDATTRIBUTEPROPERTY_HPP

#include <string>
#include <gridiron/Node.hpp>

namespace GridIron {
    namespace Controls {
        /**
         * Monitors an attribute for changes from initialization and returns a boolean
         */
        class ComparedAttributeProperty {
        public:
            friend class GridIron::Node;

            explicit ComparedAttributeProperty(const GridIron::Node* const node, const std::string a) :
                attachedNode{const_cast<Node *>(node)},
                attribute{a},
                originalData{node->getAttribute(a)}
            {}

            explicit ComparedAttributeProperty(const std::string& a) : attribute{a} {}

            // function call syntax
            bool operator()() const {
                return get();
            }

            // get/set syntax
            bool get() const {
                if (attachedNode == nullptr) return false;
                // get the current value into newValue
                std::string newValue;
                attachedNode->attribute(attribute, newValue);
                // compare against the original value
                return (originalData == newValue);
            }

            // use on rhs of '='
            explicit operator bool() const {
                return get();
            }

        protected:
            GridIron::Node *attachedNode;
            const std::string attribute;
            std::string originalData;
        };
    };
};
#endif //GRIDIRON_COMPAREDATTRIBUTEPROPERTY_HPP
