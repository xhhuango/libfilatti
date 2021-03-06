#ifndef __FILATTI_EXPOSURE_HPP__
#define __FILATTI_EXPOSURE_HPP__

#include <filatti/adjustment.hpp>
#include <filatti/dirty.hpp>
#include <filatti/synchronous.hpp>

namespace filatti {
    class Exposure : public Adjustment, public Dirty, public Synchronous {
    public:
        using Type = float;

    private:
        Type _exposure;

        cv::Mat _lut;

        void build_lut();

    public:
        static constexpr Type EXPOSURE_MAX = Type(5);
        static constexpr Type EXPOSURE_MIN = Type(-5);
        static constexpr Type EXPOSURE_NONE = Type(0);

        Exposure();

        virtual ~Exposure();

        virtual bool has_effect() const noexcept override;

        Type get_exposure() const noexcept;

        void set_exposure(Type exposure);

        virtual bool apply(const cv::Mat& src, cv::Mat& dst) override;
    };
}

#endif //__FILATTI_EXPOSURE_HPP__
