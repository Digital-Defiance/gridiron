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
 ***************************************************************************************
 * Page Class
 * ------------------
 *
 * All controls on a page must register with this.
 * Handles parsing and rendering of control classes
 * All objects on a page must register with this one
 *
 * Most control class derivatives expect to find their data in an html node
 * supplied by a Page class's parsing operation.
 ***************************************************************************************/

#ifndef _PAGE_HPP_
#define _PAGE_HPP_

// local
#include <gridiron/gridiron.hpp>
#include <gridiron/controls/control.hpp>
// STL
#include <vector>
#include <string>
#include <map>
#include <fstream>
#include <memory>

namespace GridIron
{
    class Page;

    class Control;

    // map node instances to control instances
    typedef std::map<const htmlnode *, Control *> node_map;
    // map variable names to their data
    typedef std::map<const std::string, std::string *> var_map;

    // page classes are derived from control classes. They must have no parent (NULL).
    class Page : public Control
    {
    public:
        Page(std::string frontPage);

        unique_page_ptr This();

        ~Page();

        friend std::ostream &operator<<(std::ostream &os, const Control &control);

        bool
        RegisterVariable(const std::string name, std::string *data); // register a variable for front-page access
        inline static const bool AllowAutonomous() { return false; } // can't have an autonomous page class

        static const std::string PathToPage(std::string frontPage);

        static const std::string PathToPage();

    protected:
        tree<htmlnode> _tree;      // html tree
        var_map _regvars;          // registered variables for frontpage access
        node_map _nodemap;         // registered nodes
        std::string _htmlFile;     // front page filename
        std::string _htmlFilepath; // front page filename full path
    };
}

#endif
