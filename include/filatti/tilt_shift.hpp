#ifndef __FILATTI_TILT_SHIFT_HPP__
#define __FILATTI_TILT_SHIFT_HPP__

#include <filatti/adjustment.hpp>

namespace filatti {
    class TiltShift : public Adjustment {
    public:
        enum class MaskType : unsigned int {
            CIRCULAR,
            ELLIPTIC,
            LINEAR,
        };

    private:
        cv::Point2d _center;
        double _radius;
        double _feathering;
        double _angle;
        MaskType _mask_type;

        cv::Mat _mask;
        cv::Mat _blurred;

        void create_mask(const cv::Mat& src);

    protected:
        virtual bool has_effect() const noexcept override;

    public:
        const double CENTER_MIN = 0;
        const double CENTER_MAX = 1;

        const double RADIUS_MIN = 0;
        const double RADIUS_MAX = 2;
        const double RADIUS_NONE = RADIUS_MAX;

        const double FEATHERING_MIN = 0;
        const double FEATHERING_MAX = 1;
        const double FEATHERING_NONE = FEATHERING_MIN;

        TiltShift();

        virtual ~TiltShift();

        cv::Point2d get_center() const;

        bool set_center(double x, double y);

        double get_radius() const;

        bool set_radius(double radius);

        double get_feathering() const;

        bool set_feathering(double feathering);

        double get_angle() const;

        bool set_angle(double angle);

        MaskType get_mask_type() const;

        bool set_mask_type(MaskType mask_type);

        void blur(const cv::Mat& src, cv::Mat& dst) const;

        virtual bool apply(const cv::Mat& src, cv::Mat& dst) override;
    };
}

#endif //__FILATTI_TILT_SHIFT_HPP__
