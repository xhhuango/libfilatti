#ifndef __FILATTI_HIGHLIGHT_SHADOW_HPP__
#define __FILATTI_HIGHLIGHT_SHADOW_HPP__

#include <filatti/adjustment.hpp>
#include <filatti/dirty.hpp>
#include <filatti/synchronous.hpp>

namespace filatti {
    class HighlightShadow : public Adjustment, public Dirty, public Synchronous {
    private:
        double _shadow_amount;
        double _highlight_amount;

        double _shadow_tone_width;
        double _highlight_tone_width;

        cv::Mat _lut;

        void build_lut();

    protected:
        virtual bool has_effect() const noexcept override;

    public:
        static constexpr double AMOUNT_MAX = 1;
        static constexpr double AMOUNT_MIN = -1;
        static constexpr double AMOUNT_NONE = 0;

        static constexpr double TONE_WIDTH_MAX = 0.5;
        static constexpr double TONE_WIDTH_MIN = 0;

        HighlightShadow();

        virtual ~HighlightShadow();

        double get_amount(Tone tone) const;

        void set_amount(Tone tone, double amount);

        double get_tone_width(Tone tone) const;

        void set_tone_width(Tone tone, double width);

        virtual bool apply(const cv::Mat& src, cv::Mat& dst) override;
    };
}

#endif //__FILATTI_HIGHLIGHT_SHADOW_HPP__
