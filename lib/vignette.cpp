#include <filatti/vignette.hpp>
#include <iostream>

using namespace filatti;

Vignette::Vignette()
        : _center{0.5, 0.5},
          _radius(1),
          _strength(0.8),
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

    if (!_vignette.empty())
        _vignette.release();

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

cv::Scalar Vignette::get_color() {
    return _color;
}

bool Vignette::set_color(cv::Scalar_<uchar> color) {
    _color = color;
    return true;
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

    for (int col = 0; col < width; ++col) {
        for (int row = 0; row < height; ++row) {
            double x = col - center_x;
            double y = row - center_y;
            double x_pow2 = x * x;
            double y_pow2 = y * y;

            double blend;

            if (_fit_to_image) {
                double radius_max = radius_height_pow2 - ((radius_height_pow2 * x_pow2) / radius_width_pow2);
                if (y_pow2 > radius_max) {
                    blend = _strength;
                } else {
                    radius_min = radius_max - feathering;
                    if (y_pow2 >= radius_min) {
                        blend = (y_pow2 - radius_min) / feathering;
                        blend *= _strength;
                    } else {
                        blend = 0;
                    }
                }
            } else {
                if (x_pow2 + y_pow2 > radius_circular) {
                    blend = _strength;
                } else {
                    if (x_pow2 + y_pow2 > radius_min) {
                        blend = (x_pow2 + y_pow2 - radius_min) / feathering;
                        blend *= _strength;
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
    std::vector<cv::Mat> vignette_strength_vector;
    cv::Mat vignette_strength;
    for (int i = 0, j = src.channels(); i < j; ++i)
        vignette_strength_vector.push_back(_vignette);
    cv::merge(vignette_strength_vector, vignette_strength);

    cv::Mat vignette(src.size(), src.type(), _color);

    dst = src.mul(cv::Scalar_<uchar>(255, 255, 255) - vignette_strength, 1.0 / 255.0)
          + vignette.mul(vignette_strength, 1.0 / 255.0);
}
