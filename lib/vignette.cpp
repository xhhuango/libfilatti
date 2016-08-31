#include <filatti/vignette.hpp>
#include "gradient/radial.hpp"

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

bool Vignette::has_effect() const {
    return _strength != STRENGTH_NONE && _radius != RADIUS_NONE;
}

cv::Point2d Vignette::get_center() const {
    return _center;
}

bool Vignette::set_center(double x, double y) {
    if (!within(x, CENTER_MIN, CENTER_MAX) || !within(y, CENTER_MIN, CENTER_MAX)) {
        return false;
    }

    _center = cv::Point2d(x, y);
    return true;
}

double Vignette::get_radius() const {
    return _radius;
}

bool Vignette::set_radius(double radius) {
    if (!within(radius, RADIUS_MIN, RADIUS_MAX)) {
        return false;
    }

    _radius = radius;
    return true;
}

double Vignette::get_strength() const {
    return _strength;
}

bool Vignette::set_strength(double strength) {
    if (!within(strength, STRENGTH_MIN, STRENGTH_MAX)) {
        return false;
    }

    _strength = strength;
    return true;
}

double Vignette::get_feathering() const {
    return _feathering;
}

bool Vignette::set_feathering(double feathering) {
    if (!within(feathering, FEATHERING_MIN, FEATHERING_MAX)) {
        return false;
    }

    _feathering = feathering;
    return true;
}

cv::Scalar_<uchar> Vignette::get_color() const {
    return _color;
}

bool Vignette::set_color(cv::Scalar_<uchar> color) {
    _color = color;
    return true;
}

bool Vignette::is_fit_to_image() const {
    return _fit_to_image;
}

void Vignette::set_fit_to_image(bool fit_to_image) {
    _fit_to_image = fit_to_image;
}

bool Vignette::apply(const cv::Mat& src, cv::Mat& dst) {
    if (!has_effect()) {
        return false;
    } else {
        if (!_mask.empty()) {
            _mask.release();
        }
        create_mask(src.rows, src.cols);

        blend_vignette(src, dst);
        return true;
    }
}

void Vignette::create_mask(int rows, int cols) {
    _mask.create(rows, cols, CV_8UC1);
    gradient::Radial radial(_center.x, _center.y, _radius, _feathering, _fit_to_image);
    radial.create(_mask);
}

void Vignette::blend_vignette(const cv::Mat& src, cv::Mat& dst) const {
    cv::Mat vignette_strength(src.size(), src.type());
    cv::mixChannels(_mask * _strength, vignette_strength, std::vector<int>{0, 0, 0, 1, 0, 2});

    cv::Mat vignette(src.size(), src.type(), _color);

    dst = src.mul(vignette_strength, 1.0 / 255.0)
          + vignette.mul(cv::Scalar_<uchar>(255, 255, 255) - vignette_strength, 1.0 / 255.0);
}
