#include <filatti/sharpness.hpp>
#include <filatti/contract.hpp>

#include <opencv2/opencv.hpp>

using namespace filatti;

Sharpness::Sharpness() : Dirty(true), _does_rebuild_blurred(true), _sharpness(SHARPNESS_NONE) {
}

Sharpness::~Sharpness() {
}

bool Sharpness::has_effect() const noexcept {
    return _sharpness != SHARPNESS_NONE;
}

double Sharpness::get_sharpness() const noexcept {
    return _sharpness;
}

void Sharpness::set_sharpness(double sharpness) {
    PRECONDITION(sharpness >= SHARPNESS_MIN && sharpness <= SHARPNESS_MAX, "Sharpness is out of range");
    _sharpness = sharpness;
}

bool Sharpness::does_rebuild_blurred() const noexcept {
    return _does_rebuild_blurred;
}

void Sharpness::set_rebuild_blurred(bool does_rebuild_blurred) noexcept {
    synchronize([=] {
        _does_rebuild_blurred = does_rebuild_blurred;
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
            if (_does_rebuild_blurred || _blurred.empty()) {
                build_blurred(src);
            }
        });
        cv::addWeighted(src, 1 + _sharpness, _blurred, -_sharpness, 0, dst);
        return true;
    }
}
