#ifndef GRIDIRON_COMPAREPROPERTY_HPP
#define GRIDIRON_COMPAREPROPERTY_HPP

namespace GridIron {
    namespace Properties {

        template <class T, class U = T>
        class CompareProperty {
            const T* const originalData ;
            const U* const comparedData ;
        public:
            explicit CompareProperty(const T &originalData, const U &comparedData) :
                    originalData{&originalData},
                    comparedData{&comparedData}
            {}

            // function call syntax
            bool operator()() const
            {
                return get();
            }
            // get/set syntax
            bool get() const
            {
                return (*originalData == *comparedData);
            }
            void set( T const & value ); // reserved but not implemented, per C++/CLI

            // use on rhs of '='
            explicit operator bool() const
            {
                return get();
            }

            typedef T value_type;  // might be useful for template deductions
        };
    }
}
#endif //GRIDIRON_COMPAREPROPERTY_HPP
