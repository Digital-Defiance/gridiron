#ifndef GRIDIRON_TAG_HPP
#define GRIDIRON_TAG_HPP

#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <gridiron/gridiron.hpp>

namespace GridIron {
    class Tag {
    public:
        Tag(std::string tag) : _tagName(tag) {}

        Tag(std::string tag, std::string contents) : _tagName(tag), _contents(contents) {}

        Tag(std::string tag, std::string contents, std::map<const char *, const char *> attributes) : _tagName(tag),
                                                                                                      _contents(
                                                                                                              contents),
                                                                                                      _attributes(
                                                                                                              attributes) {}

        Tag(std::string tag, std::map<const char *, const char *> attributes) : _tagName(tag),
                                                                                _attributes(attributes) {}

        inline void setAttribute(const char *attribute, const char *value) {
            this->_attributes[attribute] = value;
        }

        inline void setAttribute(std::string attribute, std::string value) {
            this->setAttribute(attribute.c_str(), value.c_str());
        }

        inline std::string tagName() {
            return _tagName;
        }

        inline std::map<const char *, const char *> attributes() {
            return _attributes;
        }

        inline const char *attribute(const char *key, const char *defaultValue) {
            std::map<const char *, const char *>::iterator it = _attributes.find(key);
            if (it != _attributes.end()) {
                return it->second;
            }
            return defaultValue;
        }

        friend inline std::ostream &operator<<(std::ostream &os, Tag &tag) {
            os << '<' << tag.tagName();
            bool empty = true;
            std::map<const char *, const char *> attrs = tag.attributes();
            for (std::map<const char *, const char *>::iterator it = attrs.begin(); it != attrs.end(); it++) {
                os << " " << it->first;
                if (it->second != nullptr) {
                    os << "=\"";
                    std::string attrValue = std::string(it->second);
                    os << xmlEncode(attrValue, os).rdbuf();
                    os << '\"';
                }
                empty = false;
            }
            if (empty) {
                os << " /";
            }
            os << '>';
            return os;
        }

    protected:
        const std::string _tagName;
        const std::string _contents;
        std::map<const char *, const char *> _attributes;
    };

}

#endif //GRIDIRON_TAG_HPP
