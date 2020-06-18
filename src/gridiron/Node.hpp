#ifndef GRIDIRON_NODE_HPP
#define GRIDIRON_NODE_HPP

#include <htmlcxx2/htmlcxx2_html.hpp>
#include <htmlcxx2/kp_tree.hh>

#include <gridiron/properties.hpp>

namespace GridIron {
    class Node : std::enable_shared_from_this<GridIron::Node>, public htmlcxx2::HTML::Node {
    public:
        /**
         * Expose the protected addAttribute, returns whether call succeeded
         * @param key
         * @param value
         * @return
         */
        inline bool newAttribute(const std::string &key, const std::string &value) {
            if (this->hasAttribute(key)) {
                return false;
            }
            // call parent
            htmlcxx2::HTML::Node::addAttribute(key, value);
            return true;
        }

        inline bool updateAttribute(const std::string &key, const std::string &value) {
            for (size_t i = 0, l = attributeKeys_.size(); i < l; ++i)
            {
                if (htmlcxx2::HTML::impl::icompare(attributeKeys_[i].c_str(), key.c_str()) == 0)
                {
                    attributeValues_[i] = value;
                    return true;
                }
            }
            // not found, try to add
            return newAttribute(key, value);
        }

        inline bool deleteAttribute(const std::string &key) {
            for (size_t i = 0, l = attributeKeys_.size(); i < l; ++i)
            {
                if (htmlcxx2::HTML::impl::icompare(attributeKeys_[i].c_str(), key.c_str()) == 0)
                {
                    attributeValues_.erase(attributeValues_.begin() + i);
                    attributeKeys_.erase(attributeKeys_.begin() + i);
                    return true;
                }
            }
            return false;
        }

        inline void updateText(const std::string &text) {
            text_ = text;
        }

        inline std::shared_ptr<Node> This() {
            return shared_from_this();
        }

        static std::shared_ptr<Node> Create() {
            return std::make_shared<Node>();
        };
    };
}

#endif //GRIDIRON_NODE_HPP
