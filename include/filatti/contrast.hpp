#ifndef __FILATTI_CONTRAST_HPP__
#define __FILATTI_CONTRAST_HPP__

#include <filatti/adjustment.hpp>
#include <filatti/dirty.hpp>
#include <filatti/synchronous.hpp>

namespace filatti {
    class Contrast : public Adjustment, public Dirty, public Synchronous {
    public:
        using Type = float;

    private:
        Type _contrast;
        cv::Mat _lut;

        void build_lut();

    public:
        static constexpr Type CONTRAST_MIN = Type(0);
        static constexpr Type CONTRAST_MAX = Type(2);
        static constexpr Type CONTRAST_NONE = Type(1);

        Contrast();

        virtual ~Contrast();

        virtual bool has_effect() const noexcept override;

        Type get_contrast() const noexcept;

        void set_contrast(Type contrast);

        virtual bool apply(const cv::Mat& src, cv::Mat& dst) override;
    };
}

#endif //__FILATTI_CONTRAST_HPP__
