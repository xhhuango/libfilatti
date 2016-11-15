#ifndef __FILATTI_WHITE_BALANCE_HPP__
#define __FILATTI_WHITE_BALANCE_HPP__

#include <filatti/adjustment.hpp>
#include <filatti/synchronous.hpp>

namespace filatti {
    class WhiteBalance : public Adjustment, public Synchronous {
    public:
        using Type = float;

    private:
        Type _percent;

        cv::Mat _lut;

        void build_lut(const cv::Mat& src);

    public:
        static constexpr Type PERCENT_MAX = Type(0.1);
        static constexpr Type PERCENT_MIN = Type(0);
        static constexpr Type PERCENT_NONE = PERCENT_NONE;

        WhiteBalance();

        virtual ~WhiteBalance();

        virtual bool has_effect() const noexcept override;

        Type get_percent() const noexcept;

        void set_percent(Type percent);

        virtual bool apply(const cv::Mat& src, cv::Mat& dst) override;
    };
}

#endif //__FILATTI_WHITE_BALANCE_HPP__
