#include <gridiron/tag.hpp>
#include <gridiron/gridiron.hpp>

namespace GridIron
{

  Tag::Tag(std::string tag) : _tagName(tag) {}

  Tag::Tag(std::string tag, std::string contents) : _tagName(tag), _contents(contents) {}

  Tag::Tag(std::string tag, std::string contents, std::map<const char *, const char *> attributes)
      : _tagName(tag), _contents(contents), _attributes(attributes) {}

  Tag::Tag(std::string tag, std::map<const char *, const char *> attributes)
      : _tagName(tag), _attributes(attributes) {}

  void Tag::setAttribute(const char *attribute, const char *value)
  {
    _attributes[attribute] = value;
  }

  void Tag::setAttribute(std::string attribute, std::string value)
  {
    setAttribute(attribute.c_str(), value.c_str());
  }

  std::string Tag::tagName()
  {
    return _tagName;
  }

  std::map<const char *, const char *> Tag::attributes()
  {
    return _attributes;
  }

  const char *Tag::attribute(const char *key, const char *defaultValue)
  {
    auto it = _attributes.find(key);
    if (it != _attributes.end())
    {
      return it->second;
    }
    return defaultValue;
  }

  std::ostream &operator<<(std::ostream &os, Tag &tag)
  {
    os << '<' << tag.tagName();
    bool empty = true;
    auto attrs = tag.attributes();
    for (const auto &it : attrs)
    {
      os << " " << it.first;
      if (it.second != nullptr)
      {
        os << "=\"";
        std::string attrValue = std::string(it.second);
        os << xmlEncode(attrValue, os).rdbuf();
        os << '\"';
      }
      empty = false;
    }
    if (empty)
    {
      os << " /";
    }
    os << '>';
    return os;
  }
}
