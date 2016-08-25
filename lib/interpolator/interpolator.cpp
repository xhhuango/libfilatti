#include "interpolator.hpp"

using namespace interpolator;

double Interpolator::operator[](const double& x) const {
    return value(x);
}

std::vector<double> Interpolator::operator[](const std::vector<double>& x) const {
    return value(x);
}
