#ifndef __FILATTI_LINEAR_HPP__
#define __FILATTI_LINEAR_HPP__

#include "interpolator.hpp"

namespace filatti {
    class Linear : public Interpolator {
    private:
        class Element {
        private:
            double _x, _y, _d;

        public:
            Element(double x);

            Element(double x, double y, double d);

            double eval(const double& x) const;

            bool operator<(const Element& e) const;

            bool operator<(const double& x) const;
        };

        typedef Element ElementType;

        std::vector<ElementType> _elements;

    public:
        Linear(const std::vector<double>& x, const std::vector<double>& y);

        ~Linear();

        virtual double value(const double& x) const override;

        virtual std::vector<double> value(const std::vector<double>& x) const override;

        virtual double operator[](const double& x) const override;

        virtual std::vector<double> operator[](const std::vector<double>& x) const override;
    };
}

#endif //__FILATTI_LINEAR_HPP__
