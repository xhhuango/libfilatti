#ifndef __FILATTI_HIGHLIGHT_SHADOW_HPP__
#define __FILATTI_HIGHLIGHT_SHADOW_HPP__

#include <filatti/adjustment.hpp>
#include <filatti/dirty.hpp>
#include <filatti/synchronous.hpp>

namespace filatti {
    class HighlightShadow : public Adjustment, public Dirty, public Synchronous {
    public:
        using Type = float;

    private:
        Type _shadow_amount;
        Type _highlight_amount;

        Type _shadow_tone_width;
        Type _highlight_tone_width;

        cv::Mat _lut;

        void build_lut();

    protected:
        virtual bool has_effect() const noexcept override;

    public:
        static constexpr Type AMOUNT_MAX = Type(1);
        static constexpr Type AMOUNT_MIN = Type(-1);
        static constexpr Type AMOUNT_NONE = Type(0);

        static constexpr Type TONE_WIDTH_MAX = Type(0.5);
        static constexpr Type TONE_WIDTH_MIN = Type(0);

        HighlightShadow();

        virtual ~HighlightShadow();

        Type get_amount(Tone tone) const noexcept;

        void set_amount(Tone tone, Type amount);

        Type get_tone_width(Tone tone) const noexcept;

        void set_tone_width(Tone tone, Type width);

        virtual bool apply(const cv::Mat& src, cv::Mat& dst) override;
    };
}

#endif //__FILATTI_HIGHLIGHT_SHADOW_HPP__
