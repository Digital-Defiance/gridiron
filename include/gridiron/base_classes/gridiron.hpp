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
 * CGICC is under GPLv3
 * Boost is under the Boost license
 ***************************************************************************************/

#ifndef _GRIDIRON_HPP_
#define _GRIDIRON_HPP_

// standard
#include <vector>
#include <string>
#include <fstream>

// htmlcxx
#include <htmlcxx/html/ParserDom.h>
#include <htmlcxx/html/utils.h>
#include <htmlcxx/css/parser_pp.h>

#ifndef GRIDIRON_XHTML_NS
#error "Please add -DGRIDIRON_XHTML_NS=gi"
#endif

#ifndef GRIDIRON_HTML_DOCROOT
#error "Please add -DGRIDIRON_HTML_DOCROOT=/path/to/gridiron/html"
#endif

namespace GridIron {
    class Control;

    // htmlnode
    typedef htmlcxx::HTML::Node htmlnode;

    const char *pathToPage(const char *frontpage);

    // in place, truncating trim (" abc d " -> "abc")
    void string_trunc_trim(std::string &in);

    // parse/find control type in <gridiron::XType ...>
    std::string gridiron_get_type(std::string &tag);

    void streamXmlEncode(std::string& data, std::ofstream &stream);
}

#endif
