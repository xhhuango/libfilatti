#include "radial.hpp"

using namespace gradient;

Radial::Radial(double center_x, double center_y, double radius, double feathering, bool is_ellipse)
        : _center_x(center_x), _center_y(center_y), _radius(radius), _feathering(feathering), _is_ellipse(is_ellipse) {
}

bool Radial::create(cv::Mat& dst) const {
    if (dst.empty() || dst.type() != CV_8UC1)
        return false;

    int width = dst.cols;
    int height = dst.rows;

    double center_x = width * _center_x;
    double center_y = height * _center_y;

    double radius_width = width * _radius;
    double radius_height = height * _radius;

    double radius_width_pow2 = radius_width * radius_width;
    double radius_height_pow2 = radius_height * radius_height;

    double feathering = (_is_ellipse)
                        ? _feathering * (radius_width * radius_height)
                        : _feathering * (radius_width < radius_height ? radius_width_pow2 : radius_height_pow2);
    double radius_circular = (radius_width < radius_height) ? radius_width_pow2 : radius_height_pow2;
    double radius_min = radius_circular - feathering;

    for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width; ++col) {
            double x = col - center_x;
            double y = row - center_y;
            double x_pow2 = x * x;
            double y_pow2 = y * y;

            double blend;

            if (_is_ellipse) {
                double radius_max = radius_height_pow2 - ((radius_height_pow2 * x_pow2) / radius_width_pow2);
                if (y_pow2 > radius_max) {
                    blend = 1;
                } else {
                    radius_min = radius_max - feathering;
                    if (y_pow2 >= radius_min) {
                        blend = (y_pow2 - radius_min) / feathering;
                    } else {
                        blend = 0;
                    }
                }
            } else {
                if (x_pow2 + y_pow2 > radius_circular) {
                    blend = 1;
                } else {
                    if (x_pow2 + y_pow2 > radius_min) {
                        blend = (x_pow2 + y_pow2 - radius_min) / feathering;
                    } else {
                        blend = 0;
                    }
                }
            }

            dst.at<uchar>(row, col) = cv::saturate_cast<uchar>(255.0 * blend);
        }
    }

    return true;
}
