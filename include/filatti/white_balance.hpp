#ifndef __FILATTI_WHITE_BALANCE_HPP__
#define __FILATTI_WHITE_BALANCE_HPP__

#include <filatti/adjustment.hpp>
#include <filatti/synchronous.hpp>

namespace filatti {
    class WhiteBalance : public Adjustment, public Synchronous {
    private:
        double _percent;

        cv::Mat _lut;

        void build_lut(const cv::Mat& src);

        void calculate_histogram(const cv::Mat& src, cv::MatND& hist) const noexcept;

        std::vector<uchar> calculate_boundary(const cv::MatND& hist,
                                              unsigned int channel,
                                              float threshold) const noexcept;

    protected:
        virtual bool has_effect() const noexcept override;

    public:
        static constexpr double PERCENT_MAX = 0.1;
        static constexpr double PERCENT_MIN = 0;
        static constexpr double PERCENT_NONE = PERCENT_NONE;

        WhiteBalance();

        ~WhiteBalance();

        double get_percent() const noexcept;

        void set_percent(double percent);

        virtual bool apply(const cv::Mat& src, cv::Mat& dst) override;
    };
}

#endif //__FILATTI_WHITE_BALANCE_HPP__
