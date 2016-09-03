#ifndef __FILATTI_ELEMENT_HPP__
#define __FILATTI_ELEMENT_HPP__

namespace filatti {
    namespace interpolator {
        class Element {
        protected:
            double _x;

        public:
            Element(double x);

            virtual ~Element() { };

            virtual double eval(const double& x) const = 0;

            bool operator<(const Element& e) const;

            bool operator<(const double& x) const;
        };
    }
}

#endif //__FILATTI_ELEMENT_HPP__
