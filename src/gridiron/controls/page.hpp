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

// STL
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <typeinfo>
#include <vector>

// local
#include <gridiron/gridiron.hpp>

namespace GridIron {
    class Control;

    // page classes are derived from control classes. They must have no parent (NULL).
    class Page : public Control {
    public:
        Page(std::string codeBesideFilename);

        ROProperty<std::shared_ptr<Page>> This;

        friend std::ostream &operator<<(std::ostream &os, const Control &control);

        bool
        RegisterVariable(const std::string name, std::string *data);    // register a variable for front-page access
        inline static const bool AllowAutonomous() { return false; }        // can't have an autonomous page class

        static const std::string PathToPage(std::string frontPage);

        static const std::string PathToPage();

        inline static const bool IsPage = true;

        inline kp::tree<htmlcxx2::HTML::Node>* HtmlTree() {
            return &_tree;
        }

        static std::shared_ptr<Page> fromHtmlNode(htmlcxx2::HTML::Node &node);

    protected:
        kp::tree<htmlcxx2::HTML::Node> _tree;        // html tree
        std::map<const std::string, RWProperty<char *>> _regvars;            // registered variables for frontpage access
        std::map<const htmlcxx2::HTML::Node *, std::shared_ptr<GridIron::Control>> _nodemap;            // registered nodes
        std::string _htmlFile;        // front page filename
        std::string _htmlFilepath;        // front page filename full path
    };
}

#endif
