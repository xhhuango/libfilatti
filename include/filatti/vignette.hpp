#ifndef __FILATTI_VIGNETTE_HPP__
#define __FILATTI_VIGNETTE_HPP__

#include "adjustment.hpp"

namespace filatti {
    class Vignette : public Adjustment {
    private:
        std::vector<double> _center;
        double _radius;
        double _strength;
        double _feathering;
        cv::Scalar_<uchar> _color;
        bool _fit_to_image;

        cv::Mat _vignette;

        void build_vignette(cv::Mat& src);
        void blend_vignette(cv::Mat& src, cv::Mat& dst);

    public:
        const double CENTER_MIN = 0;
        const double CENTER_MAX = 1;

        const double RADIUS_NONE = 1;
        const double RADIUS_MIN = 0;
        const double RADIUS_MAX = 1;

        const double STRENGTH_NONE = 0;
        const double STRENGTH_MIN = 0;
        const double STRENGTH_MAX = 1;

        const double FEATHERING_MIN = 0;
        const double FEATHERING_MAX = 1;

        Vignette();

        ~Vignette();

        std::vector<double> get_center();

        bool set_center(double x, double y);

        double get_radius();

        bool set_radius(double radius);

        double get_strength();

        bool set_strength(double strength);

        double get_feathering();

        bool set_feathering(double feathering);

        cv::Scalar get_color();

        bool set_color(cv::Scalar_<uchar> color);

        virtual bool apply(cv::Mat& src, cv::Mat& dst) override;
    };
}

#endif //__FILATTI_VIGNETTE_HPP__
