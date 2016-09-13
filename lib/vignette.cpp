#include <filatti/vignette.hpp>
#include <filatti/contract.hpp>
#include "gradient/radial.hpp"

using namespace filatti;

Vignette::Vignette() : Dirty(true), _center{0.5, 0.5}, _color{0, 0, 0}, _fit_to_image(true) {
    _radius = RADIUS_NONE;
    _strength = STRENGTH_NONE;
    _feathering = FEATHERING_NONE;
}

Vignette::~Vignette() {
}

bool Vignette::has_effect() const noexcept {
    return _strength != STRENGTH_NONE && _radius != RADIUS_NONE;
}

cv::Point2d Vignette::get_center() const noexcept {
    return _center;
}

void Vignette::set_center(const cv::Point2d& center) {
    PRECONDITION(center.x >= CENTER_MIN && center.x <= CENTER_MAX, "Center X is out of range");
    PRECONDITION(center.y >= CENTER_MIN && center.y <= CENTER_MAX, "Center Y is out of range");
    synchronize([=] {
        _center = center;
        make_dirty();
    });
}

double Vignette::get_radius() const noexcept {
    return _radius;
}

void Vignette::set_radius(double radius) {
    PRECONDITION(radius >= RADIUS_MIN && radius <= RADIUS_MAX, "Radius is out of range");
    synchronize([=] {
        _radius = radius;
        make_dirty();
    });
}

double Vignette::get_strength() const noexcept {
    return _strength;
}

void Vignette::set_strength(double strength) {
    PRECONDITION(strength >= STRENGTH_MIN && strength <= STRENGTH_MAX, "Strength is out of range");
    _strength = strength;
}

double Vignette::get_feathering() const noexcept {
    return _feathering;
}

void Vignette::set_feathering(double feathering) {
    PRECONDITION(feathering >= FEATHERING_MIN && feathering <= FEATHERING_MAX, "Feathering is out of range");
    synchronize([=] {
        _feathering = feathering;
        make_dirty();
    });
}

cv::Scalar_<uchar> Vignette::get_color() const noexcept {
    return _color;
}

void Vignette::set_color(const cv::Scalar_<uchar>& color) {
    _color = color;
}

bool Vignette::is_fit_to_image() const noexcept {
    return _fit_to_image;
}

void Vignette::set_fit_to_image(bool fit_to_image) noexcept {
    synchronize([=] {
        _fit_to_image = fit_to_image;
        make_dirty();
    });
}

bool Vignette::apply(const cv::Mat& src, cv::Mat& dst) {
    if (!has_effect()) {
        return false;
    } else {
        synchronize([this, &src] {
            if (make_clean_if_dirty()) {
                create_mask(src.size());
            }
        });

        blend_vignette(src, dst);
        return true;
    }
}

void Vignette::create_mask(const cv::Size& size) {
    _mask.create(size, CV_8UC1);
    gradient::Radial radial(_center.x, _center.y, _radius, _feathering, _fit_to_image, true);
    radial.create(_mask);
}

void Vignette::blend_vignette(const cv::Mat& src, cv::Mat& dst) const {
    cv::Mat vignette_strength(src.size(), src.type());
    cv::mixChannels(_mask * _strength, vignette_strength, std::vector<int>{0, 0, 0, 1, 0, 2});

    cv::Mat vignette(src.size(), src.type(), _color);

    dst = src.mul(cv::Scalar_<uchar>(255, 255, 255) - vignette_strength, 1.0 / 255.0)
          + vignette.mul(vignette_strength, 1.0 / 255.0);
}
