#ifndef __FILATTI_SPLINE_HPP__
#define __FILATTI_SPLINE_HPP__

#include "interpolator.hpp"
#include "element.hpp"

namespace filatti {
    namespace interpolator {
        template<typename T>
        class Spline : public Interpolator<T> {
        private:

            class SplineElement : public Element<T> {
            private:
                T _a, _b, _c, _d;

            public:
                SplineElement(T x) : Element<T>(x) { }

                SplineElement(T x, T a, T b, T c, T d) : Element<T>(x), _a(a), _b(b), _c(c), _d(d) { }

                virtual ~SplineElement() { };

                T eval(const T& x) const noexcept override {
                    T xix(x - Element<T>::_x);
                    return _a + _b * xix + _c * (xix * xix) + _d * (xix * xix * xix);
                }
            };

            using ElementType = SplineElement;

            std::vector<ElementType> _elements;

        public:
            Spline(const std::vector<T>& x, const std::vector<T>& y) {
                using SizeType = typename std::vector<T>::difference_type;

                auto n = y.size() - 1;

                std::vector<T> b(n), d(n), a(n), c(n + 1), l(n + 1), u(n + 1), z(n + 1);
                std::vector<T> h(n + 1);

                l[0] = T(1);
                u[0] = T(0);
                z[0] = T(0);
                h[0] = x[1] - x[0];

                for (SizeType i = 1; i < n; i++) {
                    h[i] = x[i + 1] - x[i];
                    l[i] = T(2 * (x[i + 1] - x[i - 1])) - T(h[i - 1]) * u[i - 1];
                    u[i] = T(h[i]) / l[i];
                    a[i] = (T(3) / T(h[i])) * (y[i + 1] - y[i]) - (T(3) / T(h[i - 1])) * (y[i] - y[i - 1]);
                    z[i] = (a[i] - T(h[i - 1]) * z[i - 1]) / l[i];
                }

                z[n] = c[n] = T(0);

                for (SizeType j = n - 1; j >= 0; j--) {
                    c[j] = z[j] - u[j] * c[j + 1];
                    b[j] = (y[j + 1] - y[j]) / T(h[j]) - (T(h[j]) * (c[j + 1] + T(2) * c[j])) / T(3);
                    d[j] = (c[j + 1] - c[j]) / T(3 * h[j]);
                }

                for (SizeType i = 0; i < n; i++) {
                    _elements.push_back(ElementType(x[i], y[i], b[i], c[i], d[i]));
                }
            }

            virtual ~Spline() { };

            virtual T value(const T& x) const noexcept override {
                typename std::vector<ElementType>::const_iterator it =
                        std::lower_bound(_elements.begin(), _elements.end(), ElementType(x));

                if (it != _elements.begin()) {
                    it--;
                }

                return it->eval(x);
            }

            virtual std::vector<T> value(const std::vector<T>& x) const noexcept override {
                typename std::vector<T>::const_iterator it;
                typename std::vector<ElementType>::const_iterator it2 = _elements.begin();
                std::vector<T> ys;
                for (it = x.begin(); it != x.end(); it++) {
                    it2 = std::lower_bound(it2, _elements.end(), ElementType(*it));
                    if (it2 != _elements.begin()) {
                        it2--;
                    }
                    ys.push_back(it2->eval(*it));
                }

                return ys;
            }
        };
    }
}

#endif //__FILATTI_SPLINE_HPP__
