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

// htmlcxx
#include <htmlcxx2/src/htmlcxx2/kp_tree.hh>
#include <htmlcxx2/src/htmlcxx2/htmlcxx2_html.hpp>
#include <gridiron/node.hpp>

#ifndef GRIDIRON_XHTML_NS
#error "Please add -DGRIDIRON_XHTML_NS=gi"
#endif

#ifndef GRIDIRON_HTML_DOCROOT
#error "Please add -DGRIDIRON_HTML_DOCROOT=/path/to/gridiron/html"
#endif

namespace GridIron {

    // parse/find control type in <gridiron::XType ...>
    std::pair<std::string, std::string> gridironParseTag(std::string tag);

    std::ostream &xmlEncode(std::string &data, std::ostream &os);

    std::ostream &xmlEncode(std::ostream &dest, std::istream &source);

    std::string xmlEncode(std::string data);

    enum PageStatus {
        INITIALIZING,   // configuring dependencies and resources
        INIT_READING,   // reading code-beside file
        INIT_PARSING,   // parsing the read HTML DOM
        PARSED_READY,   // class has finished initializing
        MATCHING,       // Matching classes to parsed nodes
        MATCHED_READY,  // logically there is another READY pause here prior to rendering being triggered
        RENDERING,      // streaming data to standard out
        RENDERED_READY  // streaming has completed
    };
}

#include <gridiron/exceptions.hpp>
#include <gridiron/property.hpp>
#include <gridiron/controls/control.hpp>
#if !defined GRIDIRON_NESTED_EXCLUDE_VALUE
#include <gridiron/controls/value.hpp>
#endif
#if !defined GRIDIRON_NESTED_EXCLUDE_PAGE
#include <gridiron/controls/page.hpp>
#endif
// Include the rest of gridiron's most primitive values

#endif
