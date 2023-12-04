#ifndef GRIDIRON_TAG_HPP
#define GRIDIRON_TAG_HPP

#include <string>
#include <map>
#include <iostream>

namespace GridIron
{
    class Tag
    {
    public:
        Tag(std::string tag);
        Tag(std::string tag, std::string contents);
        Tag(std::string tag, std::string contents, std::map<const char *, const char *> attributes);
        Tag(std::string tag, std::map<const char *, const char *> attributes);

        void setAttribute(const char *attribute, const char *value);
        void setAttribute(std::string attribute, std::string value);

        std::string tagName();
        std::map<const char *, const char *> attributes();
        const char *attribute(const char *key, const char *defaultValue);

        friend std::ostream &operator<<(std::ostream &os, Tag &tag);

    protected:
        const std::string _tagName;
        const std::string _contents;
        std::map<const char *, const char *> _attributes;
    };
}

#endif // GRIDIRON_TAG_HPP
