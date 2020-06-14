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

#ifndef _LABEL_HPP_
#define _LABEL_HPP_

/* NOTE: at this point in time, this is just a proof of concept
 * much work needs to be done in populating this
 */
#include <string>
#include <fstream>

#include <gridiron/gridiron.hpp>

namespace GridIron {
    class Control;

    class Page;
    namespace controls {
        class Label;

        class Label : public Control {
        public:
            Label(const char *id, std::shared_ptr<Control> parent, std::string text = "");
            ROProperty<std::shared_ptr<Control>, std::shared_ptr<Label>> This;
            ROProperty<ControlPass> Pass = ControlPass::FIRST;         // which pass the control is expected to be rendered on
            ROProperty<const char *> Namespace = GRIDIRON_XHTML_NS;         // gridiron namespace so it can be accessed as a regvar (needs pointed to string)
            ROProperty<const char *> RenderTag = "div";    // the associated codebeside tag name eg <namespace>::<tag>
            ROProperty<bool> AllowAutonomous = true;
            ROProperty<bool> Autonomous = false;

        private:
            // TODO: this whole approach is stuck inbetween. Need a Property/Attribute map class?
            bool _defaulttext;
            RWProperty<std::string> text; // really needs to get the node's contents?
            AttributeMappedProperty<std::string> style;
            AttributeMappedProperty<int> height;
            AttributeMappedProperty<int> width;

            // TODO: expand to handle most/all properties and deal with the overlap
            // between the properties and parsing the style argument.
            // e.g. setting height should add a style="height: Npx;"
            // but the template Tag may have a style="height: 14px;" - in which case, we have a conflict
            // the label should get the template values at instantiation, and allow to be changed before render
        };
    }
}

#endif
