#ifndef __FILATTI_BRIGHTNESS_HPP__
#define __FILATTI_BRIGHTNESS_HPP__

#include <opencv2/core.hpp>

namespace filatti {
    class Brightness {
    private:
        double _brightness;
        cv::Mat _lut;

        void build_lut();

    public:
        const double BRIGHTNESS_NONE = 0;
        const double BRIGHTNESS_MIN = -1;
        const double BRIGHTNESS_MAX = 1;

        Brightness();
        ~Brightness();
        double get_brightness();
        bool set_brightness(double brightness);
        bool apply(cv::Mat& src, cv::Mat& dst);
    };
}

#endif //__FILATTI_BRIGHTNESS_HPP__
