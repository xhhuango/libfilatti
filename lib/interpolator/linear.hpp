#ifndef __FILATTI_LINEAR_HPP__
#define __FILATTI_LINEAR_HPP__

#include "interpolator.hpp"
#include "element.hpp"

namespace interpolator {
    class Linear : public Interpolator {
    private:
        class LinearElement : public Element {
        private:
            double _y, _d;

        public:
            LinearElement(double x);

            LinearElement(double x, double y, double d);

            virtual ~LinearElement() { };

            virtual double eval(const double& x) const override;
        };

        typedef LinearElement ElementType;

        std::vector<ElementType> _elements;

    public:
        Linear(const std::vector<double>& x, const std::vector<double>& y);

        virtual ~Linear() { };

        virtual double value(const double& x) const override;

        virtual std::vector<double> value(const std::vector<double>& x) const override;
    };
}

#endif //__FILATTI_LINEAR_HPP__
