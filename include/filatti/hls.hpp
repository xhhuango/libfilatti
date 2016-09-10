#ifndef __FILATTI_BRIGHTNESS_HPP__
#define __FILATTI_BRIGHTNESS_HPP__

#include <filatti/adjustment.hpp>
#include <filatti/dirty.hpp>
#include <filatti/synchronous.hpp>

namespace filatti {
    class Hls : public Adjustment, public Dirty, public Synchronous {
    private:
        int _hue;
        double _lightness;
        double _saturation;
        cv::Mat _lut;

        void build_lut();

    protected:
        virtual bool has_effect() const noexcept override;

    public:
        const int HUE_NONE = 0;
        const int HUE_MIN = -180;
        const int HUE_MAX = 180;

        const double LIGHTNESS_NONE = 0;
        const double LIGHTNESS_MIN = -1;
        const double LIGHTNESS_MAX = 1;

        const double SATURATION_NONE = 0;
        const double SATURATION_MIN = -1;
        const double SATURATION_MAX = 1;

        Hls();

        virtual ~Hls();

        int get_hue() const noexcept;

        void set_hue(int hue);

        double get_lightness() const noexcept;

        void set_lightness(double lightness);

        double get_saturation() const noexcept;

        void set_saturation(double saturation);

        virtual bool apply(const cv::Mat& src, cv::Mat& dst) override;
    };
}

#endif //__FILATTI_BRIGHTNESS_HPP__
