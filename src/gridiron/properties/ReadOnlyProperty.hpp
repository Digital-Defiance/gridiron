#ifndef GRIDIRON_READONLYPROPERTY_HPP
#define GRIDIRON_READONLYPROPERTY_HPP


namespace GridIron {
    namespace Properties {
        template <typename T>
        class ReadOnlyProperty {
        public:
            virtual ~ReadOnlyProperty() {}
            virtual explicit operator T const & () const { return value; }
            virtual const T operator ->() const { return value; }
        protected:
            T value;
        };

        //    //    And to use it in class Owner:
//    class Owner {
//    public:
//        class : public ReadOnlyProperty<float> { friend class Owner; } x;
//        Owner() { x.value = 8; }
//    };
//
    }
}

#endif //GRIDIRON_READONLYPROPERTY_HPP
