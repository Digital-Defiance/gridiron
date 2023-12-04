/****************************************************************************************
 * (C) Copyright 2009-2023
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
#error "Please add -DGRIDIRON_XHTML_NS=GridIron"
#endif

#ifndef GRIDIRON_HTML_DOCROOT
#error "Please add -DGRIDIRON_HTML_DOCROOT=/path/to/gridiron/html"
#endif

namespace GridIron
{
    extern const std::string HtmlNamespace;

    class Control;

    // htmlnode
    typedef htmlcxx::HTML::Node htmlnode;

    // parse/find control type in <GridIron::XType ...>
    std::pair<std::string, std::string> gridironParseTag(std::string tag);

    std::string getGridIronCustomControlName(std::string tag);

    bool isCustomControl(std::string tag);

    std::ostream &xmlEncode(std::string &data, std::ostream &os);

    std::ostream &xmlEncode(std::ostream &dest, std::istream &source);

    std::string xmlEncode(std::string data);
}

#endif
