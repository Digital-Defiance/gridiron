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
        explicit ComparedAttributeProperty(GridIron::Node &n, const std::string a) :
                node{&n},
                attribute{a}
        {
            if (node->hasAttribute(attribute)) {
                // copy the original data
                node->attribute(attribute, originalData);
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
            // get the current value into newValue
            std::string newValue;
            node->attribute(attribute, newValue);
            // compare against the original value
            return (originalData == newValue);
        }

        // use on rhs of '='
        explicit operator bool() const
        {
            return get();
        }

    protected:
        GridIron::Node* node;
        const std::string attribute;
        std::string originalData;
    };
}
#endif //GRIDIRON_COMPAREDATTRIBUTEPROPERTY_HPP
