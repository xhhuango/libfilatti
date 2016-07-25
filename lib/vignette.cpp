#include <filatti/vignette.hpp>
#include <iostream>

using namespace filatti;

Vignette::Vignette()
        : _center{0.5, 0.5},
          _radius(0.6),
          _transparency(0.8),
          _feathering(0.3),
          _color{0, 0, 0},
          _fit_to_image(true) {
}

Vignette::~Vignette() { }

std::vector<double> Vignette::get_center() {
    return _center;
}

bool Vignette::set_center(double x, double y) {
    if (!within(x, CENTER_MIN, CENTER_MAX) || !within(y, CENTER_MIN, CENTER_MAX))
        return false;

    _center = std::vector<double>{x, y};
    return true;
}

double Vignette::get_radius() {
    return _radius;
}

bool Vignette::set_radius(double radius) {
    if (!within(radius, RADIUS_MIN, RADIUS_MAX))
        return false;

    _radius = radius;
    return true;
}

double Vignette::get_transparency() {
    return _transparency;
}

bool Vignette::set_transparency(double transparency) {
    if (!within(transparency, TRANSPARENCY_MIN, TRANSPARENCY_MAX))
        return false;

    _transparency = transparency;
    return true;
}

double Vignette::get_feathering() {
    return _feathering;
}

bool Vignette::set_feathering(double feathering) {
    if (!within(feathering, FEATHERING_MIN, FEATHERING_MAX))
        return false;

    _feathering = feathering;
    return true;
}

cv::Scalar Vignette::get_color() {
    return _color;
}

bool Vignette::set_color(cv::Scalar_<uchar> color) {
    _color = color;
    return true;
}

bool Vignette::apply(cv::Mat& src, cv::Mat& dst) {
    build_vignette(src);
    dst = _vignette;
    return true;
}

void Vignette::build_vignette(cv::Mat& src) {
    _vignette.create(400, 400, CV_64FC1);
    int width = _vignette.cols;
    int height = _vignette.rows;

    int center_x = width * _center[0];
    int center_y = height * _center[1];

    int radius_width = width * _radius;
    int radius_height = height * _radius;

    int radius_width_pow2 = radius_width * radius_width;
    int radius_height_pow2 = radius_height * radius_height;

    double transparency = 1.0 - _transparency;

    int feathering = (_fit_to_image)
                     ? (int) (_feathering * (radius_width * radius_height))
                     : (int) (_feathering * (radius_width < radius_height ? radius_width_pow2 : radius_height_pow2));
    int radius_circular = (radius_width < radius_height) ? radius_width_pow2 : radius_height_pow2;
    int radius_min = radius_circular - feathering;

    for (int col = 0; col < width; ++col) {
        for (int row = 0; row < height; ++row) {
            int x = col - center_x;
            int y = row - center_y;
            int x_pow2 = x * x;
            int y_pow2 = y * y;

            if (_fit_to_image) {
                int radius_max = radius_height_pow2 - ((radius_height_pow2 * x_pow2) / radius_width_pow2);
                if (y_pow2 > radius_max) {
                    _vignette.at<double>(row, col) = 0;
                } else {
                    radius_min = radius_max - feathering;
                    if (y_pow2 >= radius_min) {
                        double blend = (double) (y_pow2 - radius_min) / (double) feathering;
                        if (blend > 1.0 || blend < 0)
                            std::cout << blend;
                        _vignette.at<double>(row, col) = 1 - blend;
                    } else {
                        _vignette.at<double>(row, col) = 1.0;
                    }
                }
            } else {

            }
        }
    }
}
