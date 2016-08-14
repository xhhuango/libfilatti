#include "linear.hpp"

using namespace filatti;

Linear::Linear(const std::vector<double>& x, const std::vector<double>& y) {
    typedef std::vector<double>::difference_type SizeType;
    SizeType n = y.size() - 1;

    for (SizeType i = 0 ; i < n; ++i) {
        double d = (y[i + 1] - y[i]) / (x[i + 1] - x[i]);
        _elements.push_back(Element(x[i], y[i], d));
    }

    _elements.push_back(Element(x[n], y[n], 0));
}

Linear::~Linear() {
};

double Linear::value(const double& x) const {
    std::vector<ElementType>::const_iterator it = std::lower_bound(_elements.begin(), _elements.end(), ElementType(x));
    if (it != _elements.begin()) {
        it--;
    }

    return it->eval(x);
}

std::vector<double> Linear::value(const std::vector<double>& x) const {
    std::vector<double>::const_iterator it;
    std::vector<ElementType>::const_iterator it2 = _elements.begin();
    std::vector<double> ys;
    for (it = x.begin(); it != x.end(); it++) {
        it2 = std::lower_bound(it2, _elements.end(), ElementType(*it));
        if (it2 != _elements.begin()) {
            it2--;
        }
        ys.push_back(it2->eval(*it));
    }

    return ys;
}

double Linear::operator[](const double& x) const {
    return value(x);
}

std::vector<double> Linear::operator[](const std::vector<double>& x) const {
    return value(x);
}

Linear::Element::Element(double x) : _x(x) {
}

Linear::Element::Element(double x, double y, double d) : _x(x), _y(y), _d(d) {
}

double Linear::Element::eval(const double& x) const {
    double xix(x - _x);
    return _y + xix * _d;
}

bool Linear::Element::operator<(const Element& e) const {
    return _x < e._x;
}

bool Linear::Element::operator<(const double& x) const {
    return _x < x;
}
