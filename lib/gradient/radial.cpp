#include "radial.hpp"

using namespace filatti::gradient;

Radial::Radial(double center_x, double center_y, double radius, double feathering, bool is_elliptic, bool reserve)
        : _center_x(center_x),
          _center_y(center_y),
          _radius(radius),
          _feathering(feathering),
          _is_elliptic(is_elliptic),
          _reserve(reserve) {
}

Radial::~Radial() {
}

void Radial::create_circle(cv::Mat& dst,
                           int center_x,
                           int center_y,
                           int radius,
                           double feathering,
                           bool reserve) const {
    double radius_pow2 = radius * radius;
    double feathering_width = feathering * radius;
    double inner_radius = radius - feathering_width;
    double inner_radius_pow2 = inner_radius * inner_radius;

    for (int row = 0, height = dst.rows; row < height; ++row) {
        for (int col = 0, width = dst.cols; col < width; ++col) {
            int x = col - center_x;
            int y = row - center_y;
            int x_pow2 = x * x;
            int y_pow2 = y * y;
            double x_y_pow2 = x_pow2 + y_pow2;

            double blend;

            if (x_y_pow2 > radius_pow2) {
                blend = 0;
            } else if (x_y_pow2 > inner_radius_pow2) {
                blend = 1.0 - (std::sqrt(x_y_pow2) - inner_radius) / feathering_width;
            } else {
                blend = 1;
            }

            dst.at<uchar>(row, col) = cv::saturate_cast<uchar>(255.0 * (reserve ? 1.0 - blend : blend));
        }
    }
}

void Radial::create_ellipse(cv::Mat& dst,
                            int center_x,
                            int center_y,
                            int radius_width,
                            int radius_height,
                            double feathering,
                            bool reserve) const {
    double radius_width_pow2 = radius_width * radius_width;
    double radius_height_pow2 = radius_height * radius_height;
    double feathering_radius = 1 - feathering;

    for (int row = 0, height = dst.rows; row < height; ++row) {
        for (int col = 0, width = dst.cols; col < width; ++col) {
            int x = col - center_x;
            int y = row - center_y;
            double x_pow2 = x * x;
            double y_pow2 = y * y;

            double blend;

            double d = std::sqrt((x_pow2 / radius_width_pow2) + (y_pow2 / radius_height_pow2));
            if (d >= 1) {
                blend = 0;
            } else if (d <= feathering_radius) {
                blend = 1;
            } else {
                blend = 1 - (d - feathering_radius) / feathering;
            }

            dst.at<uchar>(row, col) = cv::saturate_cast<uchar>(255. * (reserve ? 1.0 - blend : blend));
        }
    }
}

bool Radial::create(cv::Mat& dst) const {
    if (dst.empty() || dst.type() != CV_8UC1)
        return false;

    int width = dst.cols;
    int height = dst.rows;

    int center_x = (int) (width * _center_x);
    int center_y = (int) (height * _center_y);

    int radius_width = (int) (width / 2 * _radius);
    int radius_height = (int) (height / 2 * _radius);

    if (!_is_elliptic || width == height) {
        create_circle(dst, center_x, center_y, radius_width, _feathering, _reserve);
    } else {
        create_ellipse(dst, center_x, center_y, radius_width, radius_height, _feathering, _reserve);
    }

    return true;
}
