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

#include <fmt/include/fmt/core.h>

// local
#include <gridiron/controls/Control.hpp>
#include <gridiron/ParserDom.hpp>

namespace GridIron {
    namespace Controls {
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

        // page classes are derived from control classes. They must have no parent (NULL).
        class Page : public Control {
        public:
            explicit Page(const char *codeBesideFilename);

            class : public ReadOnlyProperty<std::string> { friend class Page; } ID;       // our id
            inline static const std::string ControlType = "Page";
            inline static const std::string RenderTag = "div";    // the associated codebeside tag name eg <namespace>::<tag>
            inline static const bool IsPage = true;              // This is the one and only page!
            class : public ReadOnlyProperty <PageStatus> { friend class Page; } Status;
            class : public ReadOnlyProperty <std::string> { friend class Page; } HtmlFile;        // front page filename
            class : public ReadOnlyProperty <std::string> { friend class Page; } HtmlFilepath;    // front page filename full path

            friend std::ostream &operator<<(std::ostream &os, const Control &control);

            bool
            RegisterRWVariable(const std::string &name,
                               std::string &data);    // register a variable for front-page access
            bool
            RegisterROVariable(const std::string &name,
                               const std::string &data);    // register a variable for front-page access

            // make simpler syntax available
            inline bool
            RegisterVariable(const std::string &name, std::string& data) {
                return RegisterRWVariable(name, data);
            }

            inline bool
            RegisterVariable(const std::string &name, const std::string data) {
                return RegisterROVariable(name, data);
            }


            static const std::string PathToFile(const std::string &file);

            const std::string PathToPage();

            inline kp::tree<GridIron::Node> *HtmlTree() {
                return &_tree;
            }

//        static std::shared_ptr<Page> fromHtmlNode(htmlcxx2::HTML::Node &node);

        protected:
            GridIron::ParserDom _parser;
            Tree _tree;        // html tree
            std::map<const std::string, Property<std::string>> _regvarsRW;            // registered variables for frontpage access
            std::map<const std::string, ReadOnlyProperty<std::string>> _regvarsRO;
            std::map<const htmlcxx2::HTML::Node *, GridIron::Controls::Control*> _nodemap;            // registered nodes
        };
    };
};

#endif // PAGE_HPP
