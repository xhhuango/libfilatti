#include <filatti/sharpen.hpp>

#include <opencv2/imgproc.hpp>

using namespace filatti;

Sharpen::Sharpen() {
    _sharpen = SHARPEN_NONE;
}

Sharpen::~Sharpen() {
}

double Sharpen::get_sharpen() {
    return _sharpen;
}

bool Sharpen::set_sharpen(double sharpen) {
    if (sharpen < SHARPEN_MIN || sharpen > SHARPEN_MAX)
        return false;

    _sharpen = sharpen;

    if (!_blurred.empty())
        _blurred.release();

    return true;
}

bool Sharpen::apply(cv::Mat &src, cv::Mat &dst) {
    if (_sharpen == SHARPEN_NONE) {
        return false;
    } else {
        build_blurred(src);
        cv::addWeighted(src, 1 + _sharpen, _blurred, -_sharpen, 0, dst);
        return true;
    }
}

void Sharpen::build_blurred(cv::Mat& src) {
    if (_sharpen == SHARPEN_NONE)
        return;
    if (!_blurred.empty())
        return;

    cv::Size size(0, 0);
    cv::GaussianBlur(src, _blurred, size, 3);
}
