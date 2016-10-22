#ifndef __FILATTI_ELEMENT_HPP__
#define __FILATTI_ELEMENT_HPP__

namespace filatti {
    namespace interpolator {
        template<typename T>
        class Element {
        protected:
            T _x;

        public:
            Element(T x) : _x(x) { }

            virtual ~Element() { };

            virtual T eval(const T& x) const noexcept = 0;

            bool operator<(const Element<T>& e) const noexcept {
                return _x < e._x;
            }

            bool operator<(const T& x) const noexcept {
                return _x < x;
            }
        };
    }
}

#endif //__FILATTI_ELEMENT_HPP__
