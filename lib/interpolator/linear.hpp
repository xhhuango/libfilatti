#ifndef __FILATTI_LINEAR_HPP__
#define __FILATTI_LINEAR_HPP__

#include "interpolator.hpp"
#include "element.hpp"

namespace filatti {
    namespace interpolator {
        template<typename T>
        class Linear : public Interpolator<T> {
        private:
            class LinearElement : public Element<T> {
            private:
                T _y, _d;

            public:
                LinearElement(T x) : Element<T>(x) { }

                LinearElement(T x, T y, T d) : Element<T>(x), _y(y), _d(d) { }

                virtual ~LinearElement() { };

                T eval(const T& x) const noexcept override {
                    float xix(x - Element<T>::_x);
                    return _y + xix * _d;
                }
            };

            using ElementType = LinearElement;

            std::vector<ElementType> _elements;

        public:
            Linear(const std::vector<T>& x, const std::vector<T>& y) {
                using SizeType = typename std::vector<T>::difference_type;
                SizeType n = y.size() - 1;

                for (SizeType i = 0; i < n; ++i) {
                    T d = (y[i + 1] - y[i]) / (x[i + 1] - x[i]);
                    _elements.push_back(LinearElement(x[i], y[i], d));
                }

                _elements.push_back(LinearElement(x[n], y[n], 0));
            }

            virtual ~Linear() { };

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

#endif //__FILATTI_LINEAR_HPP__
