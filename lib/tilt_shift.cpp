#include <filatti/tilt_shift.hpp>

#include "gradient/radial.hpp"
#include "gradient/linear.hpp"

#include <opencv2/opencv.hpp>

#include <memory>

using namespace filatti;

TiltShift::TiltShift()
        : _center{0.5, 0.5},
          _angle(0.),
          _mask_type(MaskType::CIRCULAR) {
    _radius = RADIUS_NONE;
    _feathering = FEATHERING_NONE;
}

TiltShift::~TiltShift() {
}

bool TiltShift::has_effect() const noexcept {
    return _radius != RADIUS_NONE;
}

cv::Point2d TiltShift::get_center() const {
    return _center;
}

bool TiltShift::set_center(double x, double y) {
    if (!within(x, CENTER_MIN, CENTER_MAX) || !within(y, CENTER_MIN, CENTER_MAX))
        return false;

    _center = cv::Point2d(x, y);
    return true;
}

double TiltShift::get_radius() const {
    return _radius;
}

bool TiltShift::set_radius(double radius) {
    if (!within(radius, RADIUS_MIN, RADIUS_MAX))
        return false;

    _radius = radius;
    return true;
}

double TiltShift::get_feathering() const {
    return _feathering;
}

bool TiltShift::set_feathering(double feathering) {
    if (!within(feathering, FEATHERING_MIN, FEATHERING_MAX))
        return false;

    _feathering = feathering;
    return true;
}

double TiltShift::get_angle() const {
    return _angle;
}

bool TiltShift::set_angle(double angle) {
    _angle = angle;
    return true;
}

TiltShift::MaskType TiltShift::get_mask_type() const {
    return _mask_type;
}

bool TiltShift::set_mask_type(TiltShift::MaskType mask_type) {
    if (mask_type != _mask_type) {
        _mask_type = mask_type;
        return true;
    } else {
        return false;
    }
}

void TiltShift::create_mask(const cv::Mat& src) {
    if (!_mask.empty()) {
        _mask.release();
    }
    _mask.create(src.size(), CV_8UC1);

    gradient::Gradient* gradient_ptr;
    switch (_mask_type) {
        case MaskType::CIRCULAR:
            gradient_ptr = new gradient::Radial(_center.x, _center.y, _radius, _feathering, false);
            break;

        case MaskType::ELLIPTIC:
            gradient_ptr = new gradient::Radial(_center.x, _center.y, _radius, _feathering, true);
            break;

        case MaskType::LINEAR:
            gradient_ptr = new gradient::Linear(_center.x, _center.y, _radius, _feathering, _angle);
            break;
    }
    std::unique_ptr<gradient::Gradient> gradient(gradient_ptr);

    gradient->create(_mask);
}

void TiltShift::blur(const cv::Mat& src, cv::Mat& dst) const {
    cv::GaussianBlur(src, dst, cv::Size(0, 0), 3);
}

bool TiltShift::apply(const cv::Mat& src, cv::Mat& dst) {
    if (!has_effect()) {
        return false;
    } else {
        create_mask(src);

        if (!_blurred.empty()) {
            _blurred.release();
        }
        blur(src, _blurred);

        cv::Mat strength(src.size(), src.type());
        cv::mixChannels(_mask, strength, std::vector<int>{0, 0, 0, 1, 0, 2});

        dst = src.mul(strength, 1. / 255.) + _blurred.mul(cv::Scalar_<uchar>(255, 255, 255) - strength, 1. / 255.);
        return true;
    }
}
