#ifndef __FILATTI_ADJUSTMENT_HPP__
#define __FILATTI_ADJUSTMENT_HPP__

#include <opencv2/core.hpp>

namespace filatti {
    template<typename T>
    static inline bool within(T value, T min, T max) {
        return value >= min && value <= max;
    }

    class Adjustment {
    protected:
        virtual bool has_effect() const noexcept = 0;

    public:
        virtual ~Adjustment() { }

        virtual bool apply(const cv::Mat& src, cv::Mat& dst) = 0;
    };
}

#endif //__FILATTI_ADJUSTMENT_HPP__
