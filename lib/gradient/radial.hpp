#ifndef __FILATTI_RADIAL_HPP__
#define __FILATTI_RADIAL_HPP__

#include "gradient.hpp"

namespace gradient {
    class Radial : Gradient {
    private:
        double _center_x;
        double _center_y;
        double _radius;
        double _feathering;
        bool _is_ellipse;

    public:
        Radial(double center_x, double center_y, double radius, double feathering, bool is_ellipse);

        virtual ~Radial() { };

        virtual bool create(cv::Mat& dst) const override;
    };
}

#endif //__FILATTI_RADIAL_HPP__
