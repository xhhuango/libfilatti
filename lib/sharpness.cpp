#include <filatti/sharpness.hpp>

#include <opencv2/imgproc.hpp>

using namespace filatti;

Sharpness::Sharpness() {
    _sharpness = SHARPNESS_NONE;
}

Sharpness::~Sharpness() {
}

double Sharpness::get_sharpness() {
    return _sharpness;
}

bool Sharpness::set_sharpness(double sharpness) {
    if (!within(sharpness, SHARPNESS_MIN, SHARPNESS_MAX))
        return false;

    _sharpness = sharpness;

    if (sharpness == SHARPNESS_NONE && !_blurred.empty())
        _blurred.release();

    return true;
}

bool Sharpness::apply(const cv::Mat& src, cv::Mat& dst) {
    if (_sharpness == SHARPNESS_NONE) {
        return false;
    } else {
        if (_blurred.empty())
            build_blurred(src);
        cv::addWeighted(src, 1 + _sharpness, _blurred, -_sharpness, 0, dst);
        return true;
    }
}

void Sharpness::build_blurred(const cv::Mat& src) {
    cv::GaussianBlur(src, _blurred, cv::Size(0, 0), 3);
}
