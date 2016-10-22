#ifndef __FILATTI_INTERPOLATOR_HPP__
#define __FILATTI_INTERPOLATOR_HPP__

#include <vector>

namespace filatti {
    namespace interpolator {
        template <typename T>
        class Interpolator {
        public:
            virtual ~Interpolator() { }

            virtual T value(const T& x) const noexcept = 0;

            virtual std::vector<T> value(const std::vector<T>& x) const noexcept = 0;

            virtual T operator[](const T& x) const noexcept {
                return value(x);
            }

            virtual std::vector<T> operator[](const std::vector<T>& x) const noexcept {
                return value(x);
            }
        };
    }
}

#endif //__FILATTI_INTERPOLATOR_HPP__
