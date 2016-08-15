#ifndef __FILATTI_BRIGHTNESS_HPP__
#define __FILATTI_BRIGHTNESS_HPP__

#include <filatti/adjustment.hpp>

namespace filatti {
    class Hls : public Adjustment {
    private:
        double _brightness;
        double _saturation;
        cv::Mat _lut;

        bool has_effect();

        void build_lut();

        inline uchar calculate_h(int h);

        inline uchar calculate_l(int l);

        inline uchar calculate_s(int s);

    public:
        const double BRIGHTNESS_NONE = 0;
        const double BRIGHTNESS_MIN = -1;
        const double BRIGHTNESS_MAX = 1;

        const double SATURATION_NONE = 0;
        const double SATURATION_MIN = -1;
        const double SATURATION_MAX = 1;

        Hls();

        ~Hls();

        double get_brightness();

        bool set_brightness(double brightness);

        double get_saturation();

        bool set_saturation(double saturation);

        virtual bool apply(const cv::Mat& src, cv::Mat& dst) override;
    };
}

#endif //__FILATTI_BRIGHTNESS_HPP__
