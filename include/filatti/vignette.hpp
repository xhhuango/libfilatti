#ifndef __FILATTI_VIGNETTE_HPP__
#define __FILATTI_VIGNETTE_HPP__

#include "adjustment.hpp"

namespace filatti {
    class Vignette : public Adjustment {
    private:
        std::vector<double> _center;
        double _radius;
        double _transparency;
        double _feathering;
        cv::Scalar_<uchar> _color;
        bool _fit_to_image;

        cv::Mat _vignette;

        void build_vignette(cv::Mat& src);

    public:
        const double CENTER_MIN = 0;
        const double CENTER_MAX = 1;

        const double RADIUS_NONE = 100;
        const double RADIUS_MIN = 0;
        const double RADIUS_MAX = 100;

        const double TRANSPARENCY_NONE = 0;
        const double TRANSPARENCY_MIN = 0;
        const double TRANSPARENCY_MAX = 100;

        const double FEATHERING_MIN = 0;
        const double FEATHERING_MAX = 100;

        Vignette();

        ~Vignette();

        std::vector<double> get_center();

        bool set_center(double x, double y);

        double get_radius();

        bool set_radius(double radius);

        double get_transparency();

        bool set_transparency(double transparency);

        double get_feathering();

        bool set_feathering(double feathering);

        cv::Scalar get_color();

        bool set_color(cv::Scalar_<uchar> color);

        virtual bool apply(cv::Mat& src, cv::Mat& dst) override;
    };
}

#endif //__FILATTI_VIGNETTE_HPP__
