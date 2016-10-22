#ifndef __FILATTI_BRIGHTNESS_HPP__
#define __FILATTI_BRIGHTNESS_HPP__

#include <filatti/adjustment.hpp>
#include <filatti/dirty.hpp>
#include <filatti/synchronous.hpp>

namespace filatti {
    class Hls : public Adjustment, public Dirty, public Synchronous {
    public:
        using Type = float;

    private:
        int _hue;
        Type _lightness;
        Type _saturation;
        cv::Mat _lut;

        void build_lut();

    protected:
        virtual bool has_effect() const noexcept override;

    public:
        static const int HUE_NONE = 0;
        static const int HUE_MIN = -180;
        static const int HUE_MAX = 180;

        static constexpr Type LIGHTNESS_NONE = 0;
        static constexpr Type LIGHTNESS_MIN = -1;
        static constexpr Type LIGHTNESS_MAX = 1;

        static constexpr Type SATURATION_NONE = 0;
        static constexpr Type SATURATION_MIN = -1;
        static constexpr Type SATURATION_MAX = 1;

        Hls();

        virtual ~Hls();

        int get_hue() const noexcept;

        void set_hue(int hue);

        Type get_lightness() const noexcept;

        void set_lightness(Type lightness);

        Type get_saturation() const noexcept;

        void set_saturation(Type saturation);

        virtual bool apply(const cv::Mat& src, cv::Mat& dst) override;
    };
}

#endif //__FILATTI_BRIGHTNESS_HPP__
