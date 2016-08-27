#ifndef __FILATTI_SHARPNESS_HPP__
#define __FILATTI_SHARPNESS_HPP__

#include <filatti/adjustment.hpp>

namespace filatti {
    class Sharpness : public Adjustment {
    private:
        double _sharpness;
        cv::Mat _blurred;

        void build_blurred(const cv::Mat& src);

    protected:
        virtual bool has_effect() const override;

    public:
        const double SHARPNESS_MIN = 0;
        const double SHARPNESS_MAX = 20;
        const double SHARPNESS_NONE = SHARPNESS_MIN;

        Sharpness();

        virtual ~Sharpness();

        double get_sharpness() const;

        bool set_sharpness(double sharpness);

        virtual bool apply(const cv::Mat& src, cv::Mat& dst) override;
    };
}

#endif //__FILATTI_SHARPNESS_HPP__
