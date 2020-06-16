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

#include <fmt/include/fmt/printf.h>

// local
#include <gridiron/controls/Control.hpp>
#include <gridiron/properties.hpp>

namespace GridIron {
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

        ROProperty<ControlPass> Pass = ControlPass::FIRST;         // which pass the control is expected to be rendered on
        ROProperty<const char *> Namespace = GRIDIRON_XHTML_NS;         // gridiron namespace so it can be accessed as a regvar (needs pointed to string)
        ROProperty<const char *> RenderTag = "html";    // the associated codebeside tag name eg <namespace>::<tag>
        ROProperty<bool> IsPage = true;
        ROProperty<GridIron::PageStatus> Status;
        ROProperty<std::shared_ptr<Control>, std::shared_ptr<Page>> This;
        ROProperty<bool> AllowAutonomous = false; // can't have an autonomous page class
        ROProperty<bool> ViewStateEnabled = ROProperty<bool>(false);    // whether to bother serializing this object
        ROProperty<bool> ViewStateValid = ROProperty<bool>(false);      // whether viewstate was authenticated
        ROProperty<std::string> HtmlFile;        // front page filename
        ROProperty<std::string> HtmlFilepath;    // front page filename full path

        friend std::ostream &operator<<(std::ostream &os, const Control &control);

        bool
        RegisterRWVariable(const std::string &name, std::shared_ptr<std::string> data);    // register a variable for front-page access
        bool
        RegisterROVariable(const std::string &name, std::shared_ptr<const std::string> const data);    // register a variable for front-page access

        // make simpler syntax available
        inline bool
        RegisterVariable(const std::string &name, std::shared_ptr<std::string> data) {
            return RegisterRWVariable(name, data);
        }
        inline bool
        RegisterVariable(const std::string &name, std::shared_ptr<const std::string> const data) {
            return RegisterROVariable(name, data);
        }


        static const std::string PathToFile(const std::string &file);

        const std::string PathToPage();

        inline kp::tree<htmlcxx2::HTML::Node>* HtmlTree() {
            return &_tree;
        }

//        static std::shared_ptr<Page> fromHtmlNode(htmlcxx2::HTML::Node &node);

    protected:
        kp::tree<htmlcxx2::HTML::Node> _tree;        // html tree
        std::map<const std::string, RWProperty<std::shared_ptr<std::string>>> _regvarsRW;            // registered variables for frontpage access
        std::map<const std::string, ROProperty<std::shared_ptr<const std::string>>> _regvarsRO;
        std::map<const htmlcxx2::HTML::Node *, std::shared_ptr<GridIron::Control>> _nodemap;            // registered nodes
    };
}

#endif // PAGE_HPP
