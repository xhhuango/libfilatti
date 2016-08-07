#ifndef __FILATTI_CONTRAST_HPP__
#define __FILATTI_CONTRAST_HPP__

#include <filatti/adjustment.hpp>

namespace filatti {
    class Contrast : public Adjustment {
    private:
        double _contrast;
        cv::Mat _lut;

        void build_lut();

    public:
        const double CONTRAST_NONE = 1;
        const double CONTRAST_MIN = 0;
        const double CONTRAST_MAX = 2;

        Contrast();

        ~Contrast();

        double get_contrast();

        bool set_contrast(double contrast);

        virtual bool apply(const cv::Mat& src, cv::Mat& dst) override;
    };
}

#endif //__FILATTI_CONTRAST_HPP__
