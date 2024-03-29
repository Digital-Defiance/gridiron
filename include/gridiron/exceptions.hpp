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

#ifndef _EXCEPTIONS_HPP_
#define _EXCEPTIONS_HPP_

#include <string>

namespace GridIron
{
    // simple exception class to hold an id and description- likely to be replaced...
    class GridException
    {
    public:
        inline GridException(int id, const char *text)
        {
            _id = id;
            _string = std::string(text);
        };

        inline const int id() const { return _id; };

        inline const char *string() const { return _string.c_str(); };

    private:
        int _id;
        std::string _string;
    };
}

#endif
