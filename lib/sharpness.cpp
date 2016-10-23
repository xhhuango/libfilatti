#include <filatti/sharpness.hpp>
#include <filatti/contract.hpp>

#include <opencv2/opencv.hpp>

using namespace filatti;

Sharpness::Sharpness() : Rebuild(true), _sharpness(SHARPNESS_NONE) {
}

Sharpness::~Sharpness() {
}

bool Sharpness::has_effect() const noexcept {
    return _sharpness != SHARPNESS_NONE;
}

Sharpness::Type Sharpness::get_sharpness() const noexcept {
    return _sharpness;
}

void Sharpness::set_sharpness(Type sharpness) {
    PRECONDITION(sharpness >= SHARPNESS_MIN && sharpness <= SHARPNESS_MAX, "Sharpness is out of range");
    _sharpness = sharpness;
}

void Sharpness::set_rebuild_blurred(bool does_rebuild_blurred) noexcept {
    synchronize([=] {
        set_rebuild(does_rebuild_blurred);
    });
}

void Sharpness::build_blurred(const cv::Mat& src) {
    cv::GaussianBlur(src, _blurred, cv::Size(0, 0), 3);
}

bool Sharpness::apply(const cv::Mat& src, cv::Mat& dst) {
    if (!has_effect()) {
        return false;
    } else {
        synchronize([this, &src] {
            if (does_rebuild() || _blurred.empty()) {
                build_blurred(src);
            }
        });
        cv::addWeighted(src, Type(1) + _sharpness, _blurred, -_sharpness, 0, dst);
        return true;
    }
}
