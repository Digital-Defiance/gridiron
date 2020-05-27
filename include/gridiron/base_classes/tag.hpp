#ifndef GRIDIRON_TAG_HPP
#define GRIDIRON_TAG_HPP

#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <gridiron/base_classes/gridiron.hpp>

namespace GridIron {
    class Tag {
    public:
        Tag(std::string tag) : _tagName(tag) {}
        Tag(std::string tag, std::string contents) : _tagName(tag), _contents(contents) {}
        Tag(std::string tag, std::string contents, std::map<const char *, const char *> attributes) : _tagName(tag), _contents(contents), _attributes(attributes) {}
        Tag(std::string tag, std::map<const char *, const char *> attributes) : _tagName(tag), _attributes(attributes) {}
        inline void setAttribute(const char * attribute, const char * value) {
            this->_attributes[attribute] = value;
        }
        inline void setAttribute(std::string attribute, std::string value) {
            this->setAttribute(attribute.c_str(), value.c_str());
        }
        inline const char* attribute(const char *key, const char *defaultValue) {
            std::map<const char *, const char *>::iterator it = _attributes.find(key);
            if (it != _attributes.end()) {
                return it->second;
            }
            return defaultValue;
        }
        inline void render(std::ofstream &stream) {
            stream << "<" << _tagName;
            bool empty = true;
            for (std::map<const char *, const char *>::iterator it=_attributes.begin(); it != _attributes.end(); it++) {
                stream << " " << it->first;
                if (it->second != nullptr) {
                    stream << "=\"";
                    streamXmlEncode(it->second, stream);
                    stream << "\"";
                }
                empty = false;
            }
            if (empty) {
                stream << " /";
            }
            stream << ">";
        }
    protected:
        const std::string _tagName;
        const std::string _contents;
        std::map<const char *, const char *> _attributes;
    };

}

#endif //GRIDIRON_TAG_HPP
