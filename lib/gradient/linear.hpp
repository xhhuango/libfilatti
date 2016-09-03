#ifndef __FILATTI_LINEAR_HPP__
#define __FILATTI_LINEAR_HPP__

#include "gradient.hpp"

namespace filatti {
    namespace gradient {
        class Linear : public Gradient {
        private:
            double _center_x;
            double _center_y;
            double _radius;
            double _feathering;
            double _angle;

            void build_gradient(cv::Mat& gradient, double feathering) const;

        public:
            Linear(double center_x, double center_y, double radius, double feathering, double angle);

            virtual ~Linear() { }

            virtual bool create(cv::Mat& dst) const override;
        };
    }
}

#endif //__FILATTI_LINEAR_HPP__
