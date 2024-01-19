/****************************************************************************************
 * (C) Copyright 2009-2024
 *    Jessica Mulein <jessica@digitaldefiance.org>
 *    Digital Defiance and Contributors <https://digitaldefiance.org>
 *
 * Others will be credited if more developers join.
 *
 * License
 *
 * This code is licensed under the Apache license.
 * Please see COPYING in the root of this package for details.
 *
 * The following libraries are only linked in, and no code is based directly from them:
 * htmlcxx is under the Apache 2.0 License
 ***************************************************************************************/

#include <gridiron/gridiron.hpp>
#include <gridiron/exceptions.hpp>
#include <filesystem>
#include <sstream>
#include <regex>

namespace GridIron
{
    const std::string HtmlNamespace = GRIDIRON_XHTML_NS;

    // parse control type out of Tag
    std::pair<std::string, std::string> gridironParseTag(std::string tag)
    {
        std::regex rgx("^<\\s*(:alpha:+)::(:alpha:+).*>");
        std::smatch matches;
        std::regex_search(tag, matches, rgx);
        if (matches.size() == 2)
        {
            return std::make_pair<std::string, std::string>(matches[0], matches[1]);
        }
        return std::make_pair<std::string, std::string>("", "");
    }

    std::string getGridIronCustomControlName(std::string tag)
    {
        auto parsedTag = GridIron::gridironParseTag(tag);
        // check if the parsed tag is in the gridiron html namespace
        if (!parsedTag.first.empty() && parsedTag.first == HtmlNamespace && !parsedTag.second.empty())
        {
            return parsedTag.second;
        }
        return "";
    }

    bool isCustomControl(std::string tag)
    {
        return !getGridIronCustomControlName(tag).empty();
    }

    std::ostream &xmlEncode(std::ostream &dest, std::istream &source)
    {
        while (!source.eof())
        {
            char c = source.peek();
            switch (c)
            {
            case '&':
                dest << "&amp;";
                break;
            case '\"':
                dest << "&quot;";
                break;
            case '\'':
                dest << "&apos;";
                break;
            case '<':
                dest << "&lt;";
                break;
            case '>':
                dest << "&gt;";
                break;
            default:
                dest << c;
            }
        }
        return dest;
    }

    std::string xmlEncode(std::string data)
    {
        if (data.empty())
            return data;

        size_t pos;
        size_t minPos = 0;
        do
        {
            pos = data.find_first_of("&\"\'<>", minPos);
            char c = data[pos];
            switch (c)
            {
            case '&':
                data.replace(pos, 1, "&amp;");
                minPos += 5;
                break;
            case '\"':
                data.replace(pos, 1, "&qout;");
                minPos += 6;
                break;
            case '\'':
                data.replace(pos, 1, "&apos;");
                minPos += 7;
                break;
            case '<':
                data.replace(pos, 1, "&lt;");
                minPos += 4;
                break;
            case '>':
                data.replace(pos, 1, "&gt;");
                minPos += 4;
                break;
            }
        } while (pos != std::string::npos);
        return data;
    }

    std::ostream &xmlEncode(std::string data, std::ostream &os)
    {
        os << xmlEncode(data);
        return os;
    }
}
