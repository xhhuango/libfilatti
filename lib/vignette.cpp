#include <filatti/vignette.hpp>
#include <iostream>

using namespace filatti;

Vignette::Vignette()
        : _center{0.5, 0.5},
          _color{0, 0, 0},
          _fit_to_image(true) {
    _radius = RADIUS_NONE;
    _strength = STRENGTH_NONE;
    _feathering = FEATHERING_NONE;
}

Vignette::~Vignette() {
}

std::vector<double> Vignette::get_center() {
    return _center;
}

bool Vignette::set_center(double x, double y) {
    if (!within(x, CENTER_MIN, CENTER_MAX) || !within(y, CENTER_MIN, CENTER_MAX))
        return false;

    _center = std::vector<double>{x, y};

    if (!_vignette.empty())
        _vignette.release();

    return true;
}

double Vignette::get_radius() {
    return _radius;
}

bool Vignette::set_radius(double radius) {
    if (!within(radius, RADIUS_MIN, RADIUS_MAX))
        return false;

    _radius = radius;

    if (!_vignette.empty())
        _vignette.release();

    return true;
}

double Vignette::get_strength() {
    return _strength;
}

bool Vignette::set_strength(double strength) {
    if (!within(strength, STRENGTH_MIN, STRENGTH_MAX))
        return false;

    _strength = strength;
    return true;
}

double Vignette::get_feathering() {
    return _feathering;
}

bool Vignette::set_feathering(double feathering) {
    if (!within(feathering, FEATHERING_MIN, FEATHERING_MAX))
        return false;

    _feathering = feathering;

    if (!_vignette.empty())
        _vignette.release();

    return true;
}

cv::Scalar_<uchar> Vignette::get_color() {
    return _color;
}

bool Vignette::set_color(cv::Scalar_<uchar> color) {
    _color = color;
    return true;
}

bool Vignette::is_fit_to_image() {
    return _fit_to_image;
}

void Vignette::set_fit_to_image(bool fit_to_image) {
    _fit_to_image = fit_to_image;
}

bool Vignette::apply(cv::Mat& src, cv::Mat& dst) {
    if (_strength == STRENGTH_NONE || _radius == RADIUS_NONE) {
        return false;
    } else {
        if (_vignette.empty())
            build_vignette(src);
        blend_vignette(src, dst);
        return true;
    }
}

void Vignette::build_vignette(cv::Mat& src) {
    _vignette.create(src.size(), CV_8UC1);
    int width = _vignette.cols;
    int height = _vignette.rows;

    double center_x = width * _center[0];
    double center_y = height * _center[1];

    double radius_width = width * _radius;
    double radius_height = height * _radius;

    double radius_width_pow2 = radius_width * radius_width;
    double radius_height_pow2 = radius_height * radius_height;

    double feathering = (_fit_to_image)
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

            if (_fit_to_image) {
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

            _vignette.at<uchar>(row, col) = cv::saturate_cast<uchar>(255.0 * blend);
        }
    }
}

void Vignette::blend_vignette(cv::Mat& src, cv::Mat& dst) {
    cv::Mat vignette_strength(src.size(), src.type());
    cv::mixChannels(_vignette * _strength, vignette_strength, std::vector<int>{0, 0, 0, 1, 0, 2});

    cv::Mat vignette(src.size(), src.type(), _color);

    dst = src.mul(cv::Scalar_<uchar>(255, 255, 255) - vignette_strength, 1.0 / 255.0)
          + vignette.mul(vignette_strength, 1.0 / 255.0);
}
