#ifndef __FILATTI_INTERPOLATOR_HPP__
#define __FILATTI_INTERPOLATOR_HPP__

#include <vector>

namespace interpolator {
    class Interpolator {
    public:
        virtual ~Interpolator() { }

        virtual double value(const double& x) const = 0;

        virtual std::vector<double> value(const std::vector<double>& x) const = 0;

        virtual double operator[](const double& x) const;

        virtual std::vector<double> operator[](const std::vector<double>& x) const;
    };
}

#endif //__FILATTI_INTERPOLATOR_HPP__
