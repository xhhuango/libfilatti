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
        Type _offset;
        Type _gamma;

        cv::Mat _lut;

        void build_lut();

    protected:
        virtual bool has_effect() const noexcept override;

    public:
        static constexpr Type EXPOSURE_MAX = Type(5);
        static constexpr Type EXPOSURE_MIN = Type(-5);
        static constexpr Type EXPOSURE_NONE = Type(0);

        static constexpr Type OFFSET_MAX = Type(1);
        static constexpr Type OFFSET_MIN = Type(-1);
        static constexpr Type OFFSET_NONE = Type(0);

        static constexpr Type GAMMA_MIN = Type(0.01);
        static constexpr Type GAMMA_MAX = Type(2);
        static constexpr Type GAMMA_NONE = Type(1);

        Exposure();

        virtual ~Exposure();

        Type get_exposure() const noexcept;

        void set_exposure(Type exposure);

        Type get_offset() const noexcept;

        void set_offset(Type offset);

        Type get_gamma() const noexcept;

        void set_gamma(Type gamma);

        virtual bool apply(const cv::Mat& src, cv::Mat& dst) override;
    };
}

#endif //__FILATTI_EXPOSURE_HPP__
