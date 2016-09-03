#include "linear.hpp"

using namespace filatti::interpolator;

Linear::Linear(const std::vector<double>& x, const std::vector<double>& y) {
    typedef std::vector<double>::difference_type SizeType;
    SizeType n = y.size() - 1;

    for (SizeType i = 0 ; i < n; ++i) {
        double d = (y[i + 1] - y[i]) / (x[i + 1] - x[i]);
        _elements.push_back(LinearElement(x[i], y[i], d));
    }

    _elements.push_back(LinearElement(x[n], y[n], 0));
}

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

Linear::LinearElement::LinearElement(double x) : Element(x) {
}

Linear::LinearElement::LinearElement(double x, double y, double d) : Element(x), _y(y), _d(d) {
}

double Linear::LinearElement::eval(const double& x) const {
    double xix(x - _x);
    return _y + xix * _d;
}
