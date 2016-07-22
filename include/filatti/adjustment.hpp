#ifndef __FILATTI_ADJUSTMENT_HPP__
#define __FILATTI_ADJUSTMENT_HPP__

#include <opencv2/core.hpp>

namespace filatti {
    class Adjustment {
    public:
        virtual bool apply(cv::Mat& src, cv::Mat& dst) = 0;
    };
}

#endif //__FILATTI_ADJUSTMENT_HPP__
