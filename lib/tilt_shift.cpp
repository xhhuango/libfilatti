#include <filatti/tilt_shift.hpp>
#include <filatti/contract.hpp>

#include "gradient/radial.hpp"
#include "gradient/linear.hpp"

#include <memory>
#include <opencv2/opencv.hpp>

using namespace filatti;

TiltShift::TiltShift() : Dirty(true),
                         Rebuild(true),
                         _center{Type(0.5), Type(0.5)},
                         _angle(ANGLE_MIN),
                         _mask_type(MaskType::CIRCULAR),
                         _radius(RADIUS_NONE),
                         _strength(STRENGTH_NONE),
                         _feathering(FEATHERING_NONE) {
}

TiltShift::~TiltShift() {
}

bool TiltShift::has_effect() const noexcept {
    return !(_strength == STRENGTH_NONE || (_radius == RADIUS_NONE && _feathering == FEATHERING_NONE));
}

cv::Point_<TiltShift::Type> TiltShift::get_center() const noexcept {
    return _center;
}

void TiltShift::set_center(const cv::Point_<Type>& center) {
    PRECONDITION(center.x >= CENTER_MIN && center.x <= CENTER_MAX, "Center X is out of range");
    PRECONDITION(center.y >= CENTER_MIN && center.y <= CENTER_MAX, "Center Y is out of range");
    synchronize([&] {
        _center = center;
        make_dirty();
    });
}

TiltShift::Type TiltShift::get_radius() const noexcept {
    return _radius;
}

void TiltShift::set_radius(Type radius) {
    PRECONDITION(radius >= RADIUS_MIN && radius <= RADIUS_MAX, "Radius is out of range");
    synchronize([=] {
        _radius = radius;
        make_dirty();
    });
}

TiltShift::Type TiltShift::get_strength() const noexcept {
    return _strength;
}

void TiltShift::set_strength(Type strength) {
    PRECONDITION(strength >= STRENGTH_MIN && strength <= STRENGTH_MAX, "Strength is out of range");
    _strength = strength;
}

TiltShift::Type TiltShift::get_feathering() const noexcept {
    return _feathering;
}

void TiltShift::set_feathering(Type feathering) {
    PRECONDITION(feathering >= FEATHERING_MIN && feathering <= FEATHERING_MAX, "Feathering is out of range");
    synchronize([=] {
        _feathering = feathering;
        make_dirty();
    });
}

TiltShift::Type TiltShift::get_angle() const noexcept {
    return _angle;
}

void TiltShift::set_angle(Type angle) {
    PRECONDITION(angle >= ANGLE_MIN && angle <= ANGLE_MAX, "Angle is out of range");
    synchronize([=] {
        _angle = angle;
        if (_mask_type == MaskType::LINEAR) {
            make_dirty();
        }
    });
}

TiltShift::MaskType TiltShift::get_mask_type() const noexcept {
    return _mask_type;
}

void TiltShift::set_mask_type(TiltShift::MaskType mask_type) noexcept {
    synchronize([=] {
        if (mask_type != _mask_type) {
            _mask_type = mask_type;
            make_dirty();
        }
    });
}

void TiltShift::set_rebuild_blurred(bool does_rebuild_blurred) noexcept {
    synchronize([=] {
        set_rebuild(does_rebuild_blurred);
    });
}

void TiltShift::blur(const cv::Mat& src) {
    cv::GaussianBlur(src, _blurred, cv::Size(0, 0), 3);
}

bool TiltShift::apply(const cv::Mat& src, cv::Mat& dst) {
    if (!has_effect()) {
        return false;
    } else {
        synchronize([this, &src] {
            if (make_clean_if_dirty()) {
                create_mask(src);
            }
            if (does_rebuild() || _blurred.empty()) {
                blur(src);
            }
        });

        blend(src, dst);
        return true;
    }
}

void TiltShift::create_mask(const cv::Mat& src) {
    if (_mask.empty()) {
        _mask.create(src.size(), CV_8UC1);
    }

    gradient::Gradient* gradient_ptr;
    switch (_mask_type) {
        case MaskType::CIRCULAR:
            gradient_ptr = new gradient::Radial<Type>(_center.x, _center.y, _radius, _feathering, false, true);
            break;

        case MaskType::ELLIPTIC:
            gradient_ptr = new gradient::Radial<Type>(_center.x, _center.y, _radius, _feathering, true, true);
            break;

        case MaskType::LINEAR:
            gradient_ptr = new gradient::Linear<Type>(_center.x, _center.y, _radius, _feathering, _angle, true);
            break;
    }
    std::unique_ptr<gradient::Gradient> gradient(gradient_ptr);

    gradient->create(_mask);
}

void TiltShift::blend(const cv::Mat& src, cv::Mat& dst) const {
    cv::Mat mask_strength(src.size(), src.type());
    cv::mixChannels(_mask * _strength, mask_strength, std::vector<int>{0, 0, 0, 1, 0, 2});

    dst = src.mul(cv::Scalar_<uchar>(255, 255, 255) - mask_strength, Type(1) / Type(255))
          + _blurred.mul(mask_strength, Type(1) / Type(255));
}
