#ifndef __FILATTI_SHARPNESS_HPP__
#define __FILATTI_SHARPNESS_HPP__

#include <opencv2/core.hpp>

namespace filatti {
    class Sharpness {
    private:
        double _sharpness;
        cv::Mat _blurred;

        void build_blurred(cv::Mat& src);

    public:
        const double SHARPNESS_NONE = 0;
        const double SHARPNESS_MIN = SHARPNESS_NONE;
        const double SHARPNESS_MAX = 1;

        Sharpness();
        ~Sharpness();
        double get_sharpness();
        bool set_sharpness(double sharpenss);
        bool apply(cv::Mat& src, cv::Mat& dst);
    };
}

#endif //__FILATTI_SHARPNESS_HPP__
