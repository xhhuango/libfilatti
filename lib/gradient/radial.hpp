#ifndef __FILATTI_RADIAL_HPP__
#define __FILATTI_RADIAL_HPP__

#include "gradient.hpp"

namespace filatti {
    namespace gradient {
        class Radial : public Gradient {
        private:
            double _center_x;
            double _center_y;
            double _radius;
            double _feathering;
            bool _is_elliptic;

            void create_circle(cv::Mat& dst, int center_x, int center_y, int radius, double feathering) const;

            void create_ellipse(cv::Mat& dst,
                                int center_x,
                                int center_y,
                                int radius_width,
                                int radius_height,
                                double feathering) const;

        public:
            Radial(double center_x, double center_y, double radius, double feathering, bool is_elliptic);

            virtual ~Radial() { };

            virtual bool create(cv::Mat& dst) const override;
        };
    }
}

#endif //__FILATTI_RADIAL_HPP__
