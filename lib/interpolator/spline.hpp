#ifndef __FILATTI_SPLINE_HPP__
#define __FILATTI_SPLINE_HPP__

#include "interpolator.hpp"
#include "element.hpp"

namespace filatti {
    namespace interpolator {
        class Spline : public Interpolator {
        private:
            class SplineElement : public Element {
            private:
                double _a, _b, _c, _d;

            public:
                SplineElement(double x);

                SplineElement(double x, double a, double b, double c, double d);

                virtual ~SplineElement() { };

                virtual double eval(const double& x) const override;
            };

            using ElementType = SplineElement;

            std::vector<ElementType> _elements;

        public:
            Spline(const std::vector<double>& x, const std::vector<double>& y);

            virtual ~Spline() { };

            virtual double value(const double& x) const override;

            virtual std::vector<double> value(const std::vector<double>& x) const override;
        };
    }
}

#endif //__FILATTI_SPLINE_HPP__
