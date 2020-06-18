#ifndef GRIDIRON_COMPAREDATTRIBUTEPROPERTY_HPP
#define GRIDIRON_COMPAREDATTRIBUTEPROPERTY_HPP

#include <string>
#include <gridiron/Node.hpp>

namespace GridIron {
    /**
     * Monitors an attribute for changes from initialization and returns a boolean
     */
    class ComparedAttributeProperty {
    public:
        explicit ComparedAttributeProperty(std::shared_ptr<GridIron::Node> n, const std::string a) :
                node{n},
                attribute{a}
        {
            if (n->hasAttribute(a)) {
                // copy the original data
                n->attribute(a, originalData);
            }
        }

        // function call syntax
        bool operator()() const
        {
            return get();
        }
        // get/set syntax
        bool get() const
        {
            std::string newValue;
            node->attribute(attribute, newValue);
            return (originalData == newValue);
        }

        // use on rhs of '='
        explicit operator bool() const
        {
            return get();
        }

    protected:
        const std::shared_ptr<GridIron::Node> node;
        const std::string attribute;
        std::string originalData;
    };
}
#endif //GRIDIRON_COMPAREDATTRIBUTEPROPERTY_HPP
