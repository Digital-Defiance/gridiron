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
 ***************************************************************************************
 * GridIron::Label custom control class
 * ------------------------------------
 *
 * Store a text value, render it as a div
 ***************************************************************************************/

/* NOTE: at this point in time, this is just a proof of concept
 * much work needs to be done in populating this
 */

#include <gridiron/exceptions.hpp>
#include <gridiron/controls/control.hpp>
#include <gridiron/controls/page.hpp>
#include <gridiron/controls/ui/label.hpp>
#include <gridiron/tag.hpp>

using namespace GridIron;
using namespace GridIron::controls;

std::string Label::controlTagName() const
{
    return "Label";
}

std::string Label::renderTagName() const
{
    return "div";
}

Label::Label(const char *id, std::shared_ptr<Control> parent) : Control(id, parent)
{
    // nothing extra
    _text = std::string("");
    _defaulttext = true; // text has not been overriden/changed
}

Label::Label(const char *id, std::shared_ptr<Control> parent, const char *text) : Control(id, parent)
{
    // copy text
    _text = std::string(text);
    _defaulttext = false; // text has been overridden/changed
}

Label::~Label()
{
}

Label *
Label::fromHtmlNode(htmlnode &node)
{
    std::unique_ptr<Page> _Page = GetPage();
    if (_Page == nullptr)
        throw GridException(300, "Control must be attached to a page");
    if (_htmlNode == nullptr)
        throw GridException(301, "HTML Tag not found for this instance");

    // if there are child nodes, that will be the text for the label, should not override any text that has already been set.
    // if _defaulttext == true, can override
    // a shortcut to the below might be to read the parsed sibling/child

    // only parse the original/default text if we need it (it hasn't been changed)
    if (_defaulttext)
    {

        std::string starttag = _htmlNode->text();
        std::string endtag = _htmlNode->closingText();

        _text = data->substr(_htmlNode->offset() + starttag.length(),
                             _htmlNode->length() - endtag.length() - starttag.length());
    }

    // if we're an autonomous Tag, automatically register the text string for access
    // otherwise client will have to manually register if they want it accessible
    if (_autonomous)
        _Page->RegisterVariable(_id + ".Text", &_text);
}

std::ostream &operator<<(std::ostream &os, Label &label)
{
    label.parse();
    htmlcxx::HTML::Node n;
    const char *foo = "<div style=\"align: left;\">" + _text.c_str() + "</div>";
    //    return oatpp::String(foo);

    return os;
}

////////////////////////////////////////////////////////////
// Declare an instance of the proxy to register the
// existence of Label with the ControlFactory
// !! only do this for classes that support autos !!
static GridIron::ControlFactoryProxy<GridIron::controls::Label> globalLabelProxy;
