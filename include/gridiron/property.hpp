/****************************************************************************************
 * (C) Copyright 2009-2023
 *    Jessica Mulein <jessica@digitaldefiance.org>
 *    Digital Defiance and Contributors <https://digitaldefiance.org>
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
 ***************************************************************************************/

#ifndef GRIDIRON_PROPERTY_H
#define GRIDIRON_PROPERTY_H

namespace GridIron
{
    class Property
    {
        T data;

    public:
        // access with function call syntax
        Property() : data() {}

        inline T operator()() const
        {
            return data;
        }

        inline T operator()(T const &value)
        {
            data = value;
            return data;
        }

        // access with get()/set() syntax
        inline T get() const
        {
            return data;
        }

        inline T set(T const &value)
        {
            data = value;
            return data;
        }

        // access with '=' sign
        // in an industrial-strength library,
        // specializations for appropriate types might choose to
        // add combined operators like +=, etc.
        inline operator T() const
        {
            return data;
        }

        inline T operator=(T const &value)
        {
            data = value;
            return data;
        }

        typedef T value_type; // might be useful for template deductions
    };
};
#endif // GRIDIRON_PROPERTY_H
