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

#ifndef _LABEL_HPP_
#define _LABEL_HPP_

/* NOTE: at this point in time, this is just a proof of concept
 * much work needs to be done in populating this
 */

#include <gridiron/base_classes/controls/control.hpp>
#include <string>

namespace GridIron {
    class Control;

    class Page;
    namespace controls {
        class Label;

        class Label : public Control {
        public:
            Label(const char *id, Control *parent);

            Label(const char *id, Control *parent, const char *text);

            ~Label();

            virtual void parse();

            virtual oatpp::String render();

            inline void SetText(std::string value) {
                _text = value;
                _defaulttext = false;
            };    // set the text and mark it as changed
            inline std::string GetText() { return _text; };

            inline std::string *const GetTextPtr() { return &_text; };

            inline void SetHeight(int value) { _height = value; };

            inline int GetHeight() { return _height; };

            inline void SetWidth(int value) { _width = value; };

            inline int GetWidth() { return _width; };

            inline static const bool AllowAutonomous() { return true; }

            inline static const char *ClassType() { return "Label"; }        // set our type


        private:
            bool _defaulttext;
            std::string _text;
            std::string _style;
            int _height;
            int _width;

            // TODO: expand to handle most/all properties and deal with the overlap
            // between the properties and parsing the style argument.
            // e.g. setting height should add a style="height: Npx;"
            // but the template Tag may have a style="height: 14px;" - in which case, we have a conflict
            // the label should get the template values at instantiation, and allow to be changed before render
        };
    }
}

#endif
