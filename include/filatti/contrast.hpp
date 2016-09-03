#ifndef __FILATTI_CONTRAST_HPP__
#define __FILATTI_CONTRAST_HPP__

#include <filatti/adjustment.hpp>
#include <filatti/dirty.hpp>
#include <filatti/synchronous.hpp>

namespace filatti {
    class Contrast : public Adjustment, public Dirty, public Synchronous {
    private:
        double _contrast;
        cv::Mat _lut;

        void build_lut();

    protected:
        virtual bool has_effect() const noexcept override;

    public:
        const double CONTRAST_NONE = 1;
        const double CONTRAST_MIN = 0;
        const double CONTRAST_MAX = 2;

        Contrast();

        virtual ~Contrast();

        double get_contrast() const noexcept;

        void set_contrast(double contrast);

        virtual bool apply(const cv::Mat& src, cv::Mat& dst) override;
    };
}

#endif //__FILATTI_CONTRAST_HPP__
