#ifndef __FILATTI_TILT_SHIFT_HPP__
#define __FILATTI_TILT_SHIFT_HPP__

#include <filatti/adjustment.hpp>
#include <filatti/dirty.hpp>
#include <filatti/synchronous.hpp>

namespace filatti {
    class TiltShift : public Adjustment, public Dirty, public Synchronous {
    public:
        enum class MaskType : unsigned int {
            CIRCULAR = 0,
            ELLIPTIC = 1,
            LINEAR = 2,
        };

    private:
        cv::Point2d _center;
        double _radius;
        double _strength;
        double _feathering;
        double _angle;
        MaskType _mask_type;
        bool _does_rebuild_blurred;

        cv::Mat _mask;
        cv::Mat _blurred;

        void create_mask(const cv::Mat& src);

        void blur(const cv::Mat& src);

        void blend(const cv::Mat& src, cv::Mat& dst) const;

    protected:
        virtual bool has_effect() const noexcept override;

    public:
        const double CENTER_MIN = 0;
        const double CENTER_MAX = 1;

        const double RADIUS_MIN = 0;
        const double RADIUS_MAX = 2;
        const double RADIUS_NONE = RADIUS_MAX;

        const double STRENGTH_MIN = 0;
        const double STRENGTH_MAX = 1;
        const double STRENGTH_NONE = STRENGTH_MIN;

        const double FEATHERING_MIN = 0;
        const double FEATHERING_MAX = 1;
        const double FEATHERING_NONE = FEATHERING_MIN;

        const double ANGLE_MIN = 0;
        const double ANGLE_MAX = 360.0;

        TiltShift();

        virtual ~TiltShift();

        cv::Point2d get_center() const noexcept;

        void set_center(const cv::Point2d& center);

        double get_radius() const noexcept;

        void set_radius(double radius);

        double get_strength() const noexcept;

        void set_strength(double strength);

        double get_feathering() const noexcept;

        void set_feathering(double feathering);

        double get_angle() const noexcept;

        void set_angle(double angle);

        MaskType get_mask_type() const noexcept;

        bool does_rebuild_blurred() const noexcept;

        void set_rebuild_blurred(bool does_rebuild_blurred) noexcept;

        void set_mask_type(MaskType mask_type) noexcept;

        virtual bool apply(const cv::Mat& src, cv::Mat& dst) override;
    };
}

#endif //__FILATTI_TILT_SHIFT_HPP__
