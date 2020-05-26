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
 * gridiron::Label custom control class
 * ------------------------------------
 *
 * Store a text value, render it as a div
 ***************************************************************************************/

/* NOTE: at this point in time, this is just a proof of concept
 * much work needs to be done in populating this
 */

#include <gridiron/base_classes/exceptions.hpp>
#include <gridiron/base_classes/page.hpp>
#include <gridiron/base_classes/controls/ui/label.hpp>

using namespace GridIron;
using namespace GridIron::controls;

Label::Label(const char *id, Control *parent) : Control(id, parent) {
    // nothing extra
    _text = std::string("");
    _defaulttext = true;        // text has not been overriden/changed
    _classtype = std::string(Label::ClassType());
}

Label::Label(const char *id, Control *parent, const char *text) : Control(id, parent) {
    // copy text
    _text = std::string(text);
    _defaulttext = false;        // text has been overridden/changed
    _classtype = std::string(Label::ClassType());
}

Label::~Label() {
}

void
Label::parse() {
    Page *_Page = GetPage();
    if (_Page == NULL) throw GridException(300, "Control must be attached to a page");
    if (_html_node == NULL) throw GridException(301, "HTML tag not found for this instance");

    // if there are child nodes, that will be the text for the label, should not override any text that has already been set.
    // if _defaulttext == true, can override
    // a shortcut to the below might be to read the parsed sibling/child

    // only parse the original/default text if we need it (it hasn't been changed)
    if (_defaulttext) {
        std::string const *data = _Page->GetDataPtr();

        std::string starttag = _html_node->text();
        std::string endtag = _html_node->closingText();

        _text = data->substr(_html_node->offset() + starttag.length(),
                             _html_node->length() - endtag.length() - starttag.length());
    }

    // if we're an autonomous tag, automatically register the text string for access
    // otherwise client will have to manually register if they want it accessible
    if (_autonomous) _Page->RegisterVariable(_id + ".Text", &_text);
}

std::string
Label::render() {
    this->Label::parse();

    return "<div style=\"align: left;\">" + _text + "</div>";
}

////////////////////////////////////////////////////////////
// Declare an instance of the proxy to register the
// existence of Label with the ControlFactory
// !! only do this for classes that support autos !!
static GridIron::ControlFactoryProxy <GridIron::controls::Label> gLabelProxy;
