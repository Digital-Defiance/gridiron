#ifndef GRIDIRON_PROPERTY_HPP
#define GRIDIRON_PROPERTY_HPP

namespace GridIron {
    namespace Properties {
        template <typename T>
        class Property {
        public:
            virtual ~Property() {}  //C++11: use override and =default;
            virtual const T& operator= (const T& f) { return value = f; }
            virtual const T& operator() () const { return value; }
            virtual explicit operator const T& () const { return value; }
            virtual T* operator->() { return &value; }
        protected:
            T value;
        };

        //    To define a property:
        //
        //    Property<float> x;
        //    To implement a custom getter/setter just inherit:
//    class : public Property<float> {
//        virtual float & operator = (const float &f) { /*custom code*/ return value = f; }
//        virtual operator float const & () const { /*custom code*/ return value; }
//    } y;
    }
}
#endif //GRIDIRON_PROPERTY_HPP
