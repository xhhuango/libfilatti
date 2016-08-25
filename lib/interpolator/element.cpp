#include "element.hpp"

using namespace interpolator;

Element::Element(double x) : _x(x) {
}

bool Element::operator<(const Element& e) const {
    return _x < e._x;
}

bool Element::operator<(const double& x) const {
    return _x < x;
}
