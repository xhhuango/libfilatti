#ifndef __FILATTI_TILT_SHIFT_HPP__
#define __FILATTI_TILT_SHIFT_HPP__

#include <filatti/adjustment.hpp>
#include <filatti/dirty.hpp>
#include <filatti/rebuild.hpp>
#include <filatti/synchronous.hpp>

namespace filatti {
    class TiltShift : public Adjustment, public Dirty, public Rebuild, public Synchronous {
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

        cv::Mat _mask;
        cv::Mat _blurred;

        void create_mask(const cv::Mat& src);

        void blur(const cv::Mat& src);

        void blend(const cv::Mat& src, cv::Mat& dst) const;

    protected:
        virtual bool has_effect() const noexcept override;

    public:
        static constexpr double CENTER_MIN = 0;
        static constexpr double CENTER_MAX = 1;

        static constexpr double RADIUS_MIN = 0;
        static constexpr double RADIUS_MAX = 2;
        static constexpr double RADIUS_NONE = RADIUS_MAX;

        static constexpr double STRENGTH_MIN = 0;
        static constexpr double STRENGTH_MAX = 1;
        static constexpr double STRENGTH_NONE = STRENGTH_MIN;

        static constexpr double FEATHERING_MIN = 0;
        static constexpr double FEATHERING_MAX = 1;
        static constexpr double FEATHERING_NONE = FEATHERING_MIN;

        static constexpr double ANGLE_MIN = 0;
        static constexpr double ANGLE_MAX = 360.0;

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

        void set_rebuild_blurred(bool does_rebuild_blurred) noexcept;

        void set_mask_type(MaskType mask_type) noexcept;

        virtual bool apply(const cv::Mat& src, cv::Mat& dst) override;
    };
}

#endif //__FILATTI_TILT_SHIFT_HPP__
