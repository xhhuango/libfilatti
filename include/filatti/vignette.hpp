#ifndef __FILATTI_VIGNETTE_HPP__
#define __FILATTI_VIGNETTE_HPP__

#include <filatti/adjustment.hpp>

namespace filatti {
    class Vignette : public Adjustment {
    private:
        cv::Point2d _center;
        double _radius;
        double _strength;
        double _feathering;
        cv::Scalar_<uchar> _color;
        bool _fit_to_image;

        cv::Mat _vignette;

        void build_vignette(int rows, int cols);

        void blend_vignette(const cv::Mat& src, cv::Mat& dst) const;

    protected:
        virtual bool has_effect() const override;

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

        cv::Point2d get_center() const;

        bool set_center(double x, double y);

        double get_radius() const;

        bool set_radius(double radius);

        double get_strength() const;

        bool set_strength(double strength);

        double get_feathering() const;

        bool set_feathering(double feathering);

        cv::Scalar_<uchar> get_color() const;

        bool set_color(cv::Scalar_<uchar> color);

        bool is_fit_to_image() const;

        void set_fit_to_image(bool fit_to_image);

        virtual bool apply(const cv::Mat& src, cv::Mat& dst) override;
    };
}

#endif //__FILATTI_VIGNETTE_HPP__
