#ifndef __FILATTI_SPLINE_HPP__
#define __FILATTI_SPLINE_HPP__

#include "interpolator.hpp"

namespace filatti {
    class Spline : public Interpolator {
    private:
        class Element {
        private:
            double _x;
            double _a, _b, _c, _d;

        public:
            Element(double x);

            Element(double x, double a, double b, double c, double d);

            double eval(const double& x) const;

            bool operator<(const Element& e) const;

            bool operator<(const double& x) const;
        };

        typedef Element ElementType;

        std::vector<ElementType> _elements;

    public:
        Spline(const std::vector<double>& x, const std::vector<double>& y);

        ~Spline();

        virtual double value(const double& x) const override;

        virtual std::vector<double> value(const std::vector<double>& x) const override;

        virtual double operator[](const double& x) const override;

        virtual std::vector<double> operator[](const std::vector<double>& x) const override;
    };
}

#endif //__FILATTI_SPLINE_HPP__
