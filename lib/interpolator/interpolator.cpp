#include "interpolator.hpp"

using namespace filatti::interpolator;

double Interpolator::operator[](const double& x) const {
    return value(x);
}

std::vector<double> Interpolator::operator[](const std::vector<double>& x) const {
    return value(x);
}
