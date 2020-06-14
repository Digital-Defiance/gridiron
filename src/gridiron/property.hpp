/****************************************************************************************
 * From N1615 http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2004/n1615.pdf
 ***************************************************************************************/

#ifndef GRIDIRON_PROPERTY_H
#define GRIDIRON_PROPERTY_H

#include <gridiron/node.hpp>

namespace GridIron {
    template <class T>
    class BaseProperty {
    protected:
        T data;
    public:
        virtual inline T operator()() const = 0;

        virtual inline T operator()(T const &value) = 0;

        friend inline std::ostream &operator<<(std::ostream &os, BaseProperty &property);

        virtual inline T get() const = 0;

        virtual inline T set(T const &value) = 0;

        virtual inline operator T() const = 0;

        virtual T operator=(T const &value);

        typedef T value_type; // might be useful for template deductions
    };


    template <class T, class U = T>
    class RWProperty : BaseProperty<T> {
    protected:
        T data;

    public:
        // access with function call syntax
        RWProperty() : data() {}
        RWProperty(T &data) : data{reinterpret_cast<U>(data)} {}
        RWProperty(T b) : data{b} {}

        inline T operator()() const {
            return data;
        }

        inline T operator()(T const &value) {
            data = value;
            return data;
        }

        friend inline std::ostream &operator<<(std::ostream &os, RWProperty &property) {
            os << property.get();
            return os;
        }

        // access with get()/set() syntax
        inline T get() const {
            return data;
        }

        inline T set(T const &value) {
            data = value;
            return data;
        }

        // access with '=' sign
        // in an industrial-strength library,
        // specializations for appropriate types might choose to
        // add combined operators like +=, etc.
        inline operator T() const {
            return data;
        }

        inline T operator=(T const &value) {
            data = value;
            return data;
        }

        typedef T value_type; // might be useful for template deductions
    };

    template <class T, class U = T>
    class ROProperty : BaseProperty<T> {
    protected:
        T data;

    public:
        // access with function call syntax
        ROProperty() : data() {};
        ROProperty(T &data) : data{reinterpret_cast<U>(data)} {}
        ROProperty(T b) : data{b} {}

        inline T operator()() const {
            return data;
        }

        T operator()(T const &value);

        friend inline std::ostream &operator<<(std::ostream &os, ROProperty &property) {
            os << property.get();
            return os;
        }

        // access with get()/set() syntax
        inline T get() const {
            return data;
        }

        T set(T const &value); // reserved, but not implemented

        // access with '=' sign
        // in an industrial-strength library,
        // specializations for appropriate types might choose to
        // add combined operators like +=, etc.
        inline operator T() const {
            return data;
        }

        typedef T value_type; // might be useful for template deductions
    };

    template <class T>
    class AttributeMappedProperty : BaseProperty<T> {
    protected:
        const GridIron::Node node_;
        T data;
    public:
        // access with function call syntax
        AttributeMappedProperty(T attribute, GridIron::Node &node) : data{attribute}, node_{node} {}

        inline T operator()() const {
            return this->get();
        }

        inline T operator()(T const &value) {
            node_.updateAttribute(data, value);
            return data;
        }

        friend inline std::ostream &operator<<(std::ostream &os, AttributeMappedProperty &property) {
            os << property.get();
            return os;
        }

        // access with get()/set() syntax
        inline T get() const {
            if (node_.hasAttribute(data)) {
                T value;
                node_.attribute(data, value);
                return value;
            }
            return T();
        }

        inline T set(T const &value) {
            node_.updateAttribute(data, value);
            return data;
        }

        // access with '=' sign
        // in an industrial-strength library,
        // specializations for appropriate types might choose to
        // add combined operators like +=, etc.
        inline operator T() const {
            return get();
        }

        inline T operator=(T const &value) {
            node_.updateAttribute(data, value);
            return data;
        }

        typedef T value_type; // might be useful for template deductions
    };
};

#endif //GRIDIRON_PROPERTY_H
