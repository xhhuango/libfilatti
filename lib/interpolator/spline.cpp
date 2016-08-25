#include "spline.hpp"

using namespace interpolator;

Spline::Spline(const std::vector<double>& x, const std::vector<double>& y) {
    typedef std::vector<double>::difference_type SizeType;

    SizeType n = y.size() - 1;

    std::vector<double> b(n), d(n), a(n), c(n + 1), l(n + 1), u(n + 1), z(n + 1);
    std::vector<double> h(n + 1);

    l[0] = double(1);
    u[0] = double(0);
    z[0] = double(0);
    h[0] = x[1] - x[0];

    for (SizeType i = 1; i < n; i++) {
        h[i] = x[i + 1] - x[i];
        l[i] = double(2 * (x[i + 1] - x[i - 1])) - double(h[i - 1]) * u[i - 1];
        u[i] = double(h[i]) / l[i];
        a[i] = (double(3) / double(h[i])) * (y[i + 1] - y[i]) - (double(3) / double(h[i - 1])) * (y[i] - y[i - 1]);
        z[i] = (a[i] - double(h[i - 1]) * z[i - 1]) / l[i];
    }

    l[n] = double(1);
    z[n] = c[n] = double(0);

    for (SizeType j = n - 1; j >= 0; j--) {
        c[j] = z[j] - u[j] * c[j + 1];
        b[j] = (y[j + 1] - y[j]) / double(h[j]) - (double(h[j]) * (c[j + 1] + double(2) * c[j])) / double(3);
        d[j] = (c[j + 1] - c[j]) / double(3 * h[j]);
    }

    for (SizeType i = 0; i < n; i++) {
        _elements.push_back(ElementType(x[i], y[i], b[i], c[i], d[i]));
    }
}

double Spline::value(const double& x) const {
    std::vector<ElementType>::const_iterator it = std::lower_bound(_elements.begin(), _elements.end(), ElementType(x));
    if (it != _elements.begin()) {
        it--;
    }

    return it->eval(x);
}

std::vector<double> Spline::value(const std::vector<double>& x) const {
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

Spline::SplineElement::SplineElement(double x) : Element(x) {
}

Spline::SplineElement::SplineElement(double x, double a, double b, double c, double d)
        : Element(x), _a(a), _b(b), _c(c), _d(d) {
}

double Spline::SplineElement::eval(const double& x) const {
    double xix(x - _x);
    return _a + _b * xix + _c * (xix * xix) + _d * (xix * xix * xix);
}
