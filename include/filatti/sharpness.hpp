#ifndef __FILATTI_SHARPNESS_HPP__
#define __FILATTI_SHARPNESS_HPP__

#include <filatti/adjustment.hpp>

namespace filatti {
    class Sharpness : public Adjustment {
    private:
        double _sharpness;
        cv::Mat _blurred;

        void build_blurred(const cv::Mat& src);

    public:
        const double SHARPNESS_NONE = 0;
        const double SHARPNESS_MIN = SHARPNESS_NONE;
        const double SHARPNESS_MAX = 1;

        Sharpness();

        ~Sharpness();

        double get_sharpness();

        bool set_sharpness(double sharpness);

        virtual bool apply(const cv::Mat& src, cv::Mat& dst) override;
    };
}

#endif //__FILATTI_SHARPNESS_HPP__
