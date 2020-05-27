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
#include <gridiron/base_classes/gridiron.hpp>
#include <gridiron/base_classes/controls/control.hpp>
// STL
#include <vector>
#include <string>
#include <map>

namespace GridIron {
    class Page;

    class Control;

    // map node instances to control instances
    typedef std::map<const htmlnode *, Control *> node_map;
    // map variable names to their data
    typedef std::map<const std::string, std::string *> var_map;

    // page classes are derived from control classes. They must have no parent (NULL).
    class Page : public Control {
    public:
        Page(const char *frontpage);

        ~Page();

        virtual void parse();                                                // parse the underlying html
        virtual oatpp::String render();                                        // recursively render all children and output
        bool
        RegisterVariable(const std::string name, std::string *data);    // register a variable for front-page access
        inline std::string *
        GetDataPtr() { return &_data; };                // return a pointer to the raw html file's data
        inline static const bool AllowAutonomous() { return false; }        // can't have an autonomous page class
        inline static const char *ClassType() { return "Page"; };            // we're a Page type

    protected:
        std::string _data;            // raw page data
        tree<htmlnode> _tree;        // html tree
        var_map _regvars;            // registered variables for frontpage access
        node_map _nodemap;            // registered nodes
        std::string _htmlfile;        // front page filename
        std::string _htmlfilepath;        // front page filename full path
        static const std::string _namespace;        // gridiron namespace so it can be accessed as a regvar (needs pointed to string)
    private:
        void renderNode(tree<htmlnode>::sibling_iterator *thisnode, int level,
                        oatpp::String &rendered);    // helper function for recursing the node tree
    };
}

#endif
