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

#include <cstdio>
#include <iostream>
#include <gridiron/base_classes/gridiron.hpp>

namespace GridIron {
    // in place, truncating trim (" abc d " -> "abc")
    void string_trunc_trim(std::string &in) {
        std::string::size_type idx;

        // trim trailing spaces (if any)
        idx = in.find_last_not_of(' ');
        if (idx != std::string::npos) {
            in.erase(idx + 1);
            idx = in.find_first_not_of(' ');
            if (idx != std::string::npos)
                in.erase(0, idx);
        } else {
            in.erase(in.begin(), in.end());
        }
    }

    // parse control type out of tag
    std::string gridiron_get_type(std::string &tag) {
        // assemble namespace string from config.h constant
        std::string nstr = std::string("<") + std::string(GRIDIRON_XHTML_NS) + std::string("::");

        // try to find it
        int gridironpos = tag.find(nstr);
        if (gridironpos == std::string::npos)
            return "";

        int firstspace = tag.find(' ');
        if (firstspace == std::string::npos)
            return "";

        return tag.substr(nstr.length(), (firstspace - nstr.length()));
    }
}
