#include <filatti/sharpness.hpp>

#include <opencv2/opencv.hpp>

using namespace filatti;

Sharpness::Sharpness() {
    _sharpness = SHARPNESS_NONE;
}

Sharpness::~Sharpness() {
}

bool Sharpness::has_effect() const noexcept {
    return _sharpness != SHARPNESS_NONE;
}

double Sharpness::get_sharpness() const {
    return _sharpness;
}

bool Sharpness::set_sharpness(double sharpness) {
    if (!within(sharpness, SHARPNESS_MIN, SHARPNESS_MAX)) {
        return false;
    }

    _sharpness = sharpness;
    return true;
}

void Sharpness::blur(const cv::Mat& src, cv::Mat& dst) const {
    cv::GaussianBlur(src, dst, cv::Size(0, 0), 3);
}

bool Sharpness::apply(const cv::Mat& src, cv::Mat& dst) {
    if (!has_effect()) {
        return false;
    } else {
        if (!_blurred.empty()) {
            _blurred.release();
        }
        blur(src, _blurred);

        cv::addWeighted(src, 1 + _sharpness, _blurred, -_sharpness, 0, dst);
        return true;
    }
}
