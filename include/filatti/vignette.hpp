#ifndef __FILATTI_VIGNETTE_HPP__
#define __FILATTI_VIGNETTE_HPP__

#include <filatti/adjustment.hpp>
#include <filatti/dirty.hpp>
#include <filatti/synchronous.hpp>

namespace filatti {
    class Vignette : public Adjustment, public Dirty, public Synchronous {
    private:
        cv::Point2d _center;
        double _radius;
        double _strength;
        double _feathering;
        cv::Scalar_<uchar> _color;
        bool _fit_to_image;

        cv::Mat _mask;

        void create_mask(const cv::Size& size);

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

        Vignette();

        virtual ~Vignette();

        cv::Point2d get_center() const noexcept;

        void set_center(const cv::Point2d& center);

        double get_radius() const noexcept;

        void set_radius(double radius);

        double get_strength() const noexcept;

        void set_strength(double strength);

        double get_feathering() const noexcept;

        void set_feathering(double feathering);

        cv::Scalar_<uchar> get_color() const noexcept;

        void set_color(const cv::Scalar_<uchar>& color);

        bool is_fit_to_image() const noexcept;

        void set_fit_to_image(bool fit_to_image) noexcept;

        virtual bool apply(const cv::Mat& src, cv::Mat& dst) override;
    };
}

#endif //__FILATTI_VIGNETTE_HPP__
