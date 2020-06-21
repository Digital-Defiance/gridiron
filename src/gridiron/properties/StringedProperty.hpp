#ifndef GRIDIRON_PROPERTY_HPP
#define GRIDIRON_PROPERTY_HPP

#include <string>
#include <nall/string.hpp>

namespace GridIron {
    namespace Properties {
        template<typename T, bool settable = true>
        class StringedProperty {
        public:
            StringedProperty(T *value) {
                if (value != nullptr) {
                    _value = *value; // copy value
                    _hasValue = true;
                }
            }

            virtual ~StringedProperty() {}  //C++11: use override and =default;

            template<class S>
            const std::enable_if_t<settable, T> operator=(const T &f) {
                _value = f;
                _hasValue = true;
                return _value;
            }

            virtual const std::string &operator()() const { return this->get(); }

            virtual explicit operator const std::string&() const { return this->get(); }

            virtual std::string operator->() {
                return _hasValue ? this->get(): nullptr;
            }

            virtual const std::string get() {
                return _hasValue ? nall::stringify<T>(_value) : "";
            }

            typedef T value_type;
        protected:
            T _value;
            bool _hasValue = false;
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
