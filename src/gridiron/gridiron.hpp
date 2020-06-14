/****************************************************************************************
 * (C) Copyright 2009-2020
 *    Jessica Mulein <jessica@mulein.com>
 *
 * Others will be credited if more developers join.
 *
 * License
 *
 * This code is licensed under the GPLv3.
 * Please see COPYING in the root of this package for details.
 *
 * The following libraries are only linked in, and no code is based directly from them:
 * htmlcxx is under the Apache 2.0 License
 ***************************************************************************************/

#ifndef _GRIDIRON_HPP_
#define _GRIDIRON_HPP_

// standard
#include <filesystem>
#include <fstream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#ifndef GRIDIRON_XHTML_NS
#error "Please add -DGRIDIRON_XHTML_NS=gi"
#endif

#ifndef GRIDIRON_HTML_DOCROOT
#error "Please add -DGRIDIRON_HTML_DOCROOT=/path/to/gridiron/html"
#endif

namespace GridIron {

    // parse/find control type in <gridiron::XType ...>
    static std::pair<std::string, std::string> gridironParseTag(std::string tag);

    std::ostream &xmlEncode(std::string &data, std::ostream &os);

    std::ostream &xmlEncode(std::ostream &dest, std::istream &source);

    std::string xmlEncode(std::string data);

    template<typename Base, typename T = Base>
    static inline bool instanceOf(std::shared_ptr<Base> p) {
        std::shared_ptr<Base> temp = std::dynamic_pointer_cast<T>(p);
        return temp.get() != nullptr;
    }
}

#endif
