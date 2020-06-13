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
 * gridiron::Label custom control class
 * ------------------------------------
 *
 * Store a text value, render it as a div
 ***************************************************************************************/

/* NOTE: at this point in time, this is just a proof of concept
 * much work needs to be done in populating this
 */

#include <gridiron/controls/ui/label.hpp>

namespace GridIron {
    using namespace GridIron::controls;

    Label::Label(const char *id, std::shared_ptr<Control> parent, std::string text) : Control(id, parent) :
        Namespace{GRIDIRON_XHTML_NS},
        tagName_{GRIDIRON_XHTML_NS + "::Label"},
        RenderTag{"div"}
    {
        // copy text
        _text = std::string(text);
        _defaulttext = text.empty();
        parent->GetPage().RegisterVariable(_id.append(".Text"), lblTest.GetTextPtr());
    }

    static
    std::shared_ptr<Label>
    Label::fromHtmlNode(htmlcxx2::HTML::Node &node) {
        std::shared_ptr<Page> _Page = GetPage();
        if (_Page == nullptr) throw GridException(300, "Control must be attached to a page");

        // if there are child nodes, that will be the text for the label, should not override any text that has already been set.
        // if _defaulttext == true, can override
        // a shortcut to the below might be to read the parsed sibling/child

        // only parse the original/default text if we need it (it hasn't been changed)
        if (_defaulttext) {

            std::string starttag = node->text();
            std::string endtag = node->closingText();

            _text = data->substr(node->offset() + starttag.length(),
                                 node->length() - endtag.length() - starttag.length());
        }

        // if we're an autonomous Tag, automatically register the text string for access
        // otherwise client will have to manually register if they want it accessible
        if (_autonomous) _Page->RegisterVariable(_id + ".Text", &_text);
    }


    friend std::ostream& operator<<(std::ostream& os, Label& label) {
        label.parse();
        htmlcxx2::HTML::Node n;
        // TODO: use namespace::tagname, render out all the HTML attributes
        const char* foo = "<div style=\"align: left;\">" + label. _text.c_str() + "</div>";
    //    return oatpp::String(foo);

        return os;
    }

    ////////////////////////////////////////////////////////////
    // Declare an instance of the proxy to register the
    // existence of Label with the ControlFactory
    // !! only do this for classes that support autos !!
    static GridIron::ControlFactoryProxy<GridIron::controls::Label> globalLabelProxy;
}