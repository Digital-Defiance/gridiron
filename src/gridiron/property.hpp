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

        inline const T* getPointerRO() const {
            return &data;
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
    class ROIndirectProperty {
        std::shared_ptr<const T> const data ;
    public:
        explicit ROIndirectProperty(const T &data) : data{std::shared_ptr<const T>(&data)} {}

        inline std::shared_ptr<const T> ROPointer() const {
            return std::shared_ptr<const T>(data);
        }

        // this function must be called by the containing class, normally in a
        // constructor, to initialize the ROProperty so it knows where its
        // real implementation code can be found. obj is usually the containing
        // class, but need not be; it could be a special implementation object.
        void operator () ( std::shared_ptr<const T> t )
        {
            data = t;
        }
        // function call syntax
        T operator()() const
        {
            return data->get();
        }
        // get/set syntax
        T get() const
        {
            return data->get();
        }
        void set( T const & value ); // reserved but not implemented, per C++/CLI

        // use on rhs of '='
        operator T() const
        {
            return data->get();
        }

        typedef T value_type;  // might be useful for template deductions
    };

    template <class T, class U = T>
    class CompareProperty {
        std::shared_ptr<const T> const originalData ;
        std::shared_ptr<const T> const comparedData ;
    public:
        explicit CompareProperty(const T &originalData, const U &comparedData) :
            originalData{std::shared_ptr<const T>(&originalData)},
            comparedData{std::shared_ptr<const T>(&comparedData)}
        {}

        // function call syntax
        bool operator()() const
        {
            return (originalData.get() == comparedData.get());
        }
        // get/set syntax
        bool get() const
        {
            return (originalData.get() == comparedData.get());
        }
        void set( T const & value ); // reserved but not implemented, per C++/CLI

        // use on rhs of '='
        operator bool() const
        {
            return (originalData.get() == comparedData.get());
        }

        typedef T value_type;  // might be useful for template deductions
    };

    template <class T>
    class AttributeMappedProperty : BaseProperty<T> {
    protected:
        const GridIron::Node node_;
        bool changed_ = false;
        T original;
        T data;
    public:
        // access with function call syntax
        AttributeMappedProperty(T attributeData, GridIron::Node &node) : data{attributeData}, node_{node} {
            // locate the original attribute value and source the value
            if (node.hasAttribute(std::to_string(attributeData))) {
                std::string value;
                node.attribute(std::to_string(attributeData), value);
                this->original = value;
                this->changed_ = (this->original != this->data);
            } else {
                this->original = this->data;
                this->changed_ = false;
            }
        }
        AttributeMappedProperty(AttributeMappedProperty &property) {
            this->node_ = property.node_;
            this->original = property.original;
            this->data = property.data;
            this->changed_ = false;
        }

        inline T operator()() const {
            return this->get();
        }

        inline T operator()(T const &value) {
            node_.updateAttribute(std::to_string(data), value);
            return data;
        }

        friend inline std::ostream &operator<<(std::ostream &os, AttributeMappedProperty &property) {
            os << property.get();
            return os;
        }

        // access with get()/set() syntax
        inline T get() const {
            if (node_.hasAttribute(std::to_string(data))) {
                T value;
                node_.attribute(std::to_string(data), value);
                return value;
            }
            return T();
        }

        inline T set(T const &value) {
            node_.updateAttribute(std::to_string(data), value);
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
            node_.updateAttribute(std::to_string(data), value);
            return data;
        }

        typedef T value_type; // might be useful for template deductions
    };
};

#endif //GRIDIRON_PROPERTY_H
