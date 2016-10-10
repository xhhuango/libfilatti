#ifndef __FILATTI_COLOR_BALANCE_HPP__
#define __FILATTI_COLOR_BALANCE_HPP__

#include <filatti/adjustment.hpp>
#include <filatti/dirty.hpp>
#include <filatti/synchronous.hpp>

namespace filatti {
    class ColorBalance : public Adjustment, public Dirty, public Synchronous {
    private:
        static constexpr ToneRawType TONES = static_cast<ToneRawType>(Tone::TOTAL);
        static constexpr ToneRawType SHADOWS = static_cast<ToneRawType>(Tone::SHADOWS);
        static constexpr ToneRawType MIDTONES = static_cast<ToneRawType>(Tone::MIDTONES);
        static constexpr ToneRawType HIGHLIGHTS = static_cast<ToneRawType>(Tone::HIGHLIGHTS);

        int _red_cyan[TONES];
        int _green_magenta[TONES];
        int _blue_yellow[TONES];

        bool _is_add_sub_built;
        cv::Mat _shadows_add_lut;
        cv::Mat _shadows_sub_lut;
        cv::Mat _midtones_add_lut;
        cv::Mat _midtones_sub_lut;
        cv::Mat _highlights_add_lut;
        cv::Mat _highlights_sub_lut;

        cv::Mat _lut;

        void compute(int add[TONES], int sub1[TONES], int sub2[TONES], int res[TONES]);

        void build_add_sub_luts();

        void build_lut();

    protected:
        virtual bool has_effect() const noexcept override;

    public:
        static const int VALUE_MAX = 100;
        static const int VALUE_MIN = -100;
        static const int VALUE_NONE = 0;

        ColorBalance();

        virtual ~ColorBalance();

        int get_red_cyan(Tone tone) const noexcept;

        void set_red_cyan(Tone tone, int value);

        int get_green_magenta(Tone tone) const noexcept;

        void set_green_magenta(Tone tone, int value);

        int get_blue_yellow(Tone tone) const noexcept;

        void set_blue_yellow(Tone tone, int value);

        virtual bool apply(const cv::Mat& src, cv::Mat& dst) override;
    };
}

#endif //__FILATTI_COLOR_BALANCE_HPP__
