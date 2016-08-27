#include <filatti/contrast.hpp>

using namespace filatti;

Contrast::Contrast() {
    _contrast = CONTRAST_NONE;
}

Contrast::~Contrast() {
}

bool Contrast::has_effect() const {
    return _contrast != CONTRAST_NONE;
}

double Contrast::get_contrast() const {
    return _contrast;
}

bool Contrast::set_contrast(double contrast) {
    if (!within(contrast, CONTRAST_MIN, CONTRAST_MAX))
        return false;

    _contrast = contrast;
    build_lut();
    return true;
}

bool Contrast::apply(const cv::Mat& src, cv::Mat& dst) {
    if (!has_effect()) {
        return false;
    } else {
        cv::LUT(src, _lut, dst);
        return true;
    }
}

void Contrast::build_lut() {
    if (_contrast == CONTRAST_NONE) {
        if (!_lut.empty())
            _lut.release();
        return;
    }

    cv::Mat lut(256, 1, CV_8UC1);
    uchar* p = lut.data;
    for (int i = 0; i < 256; ++i)
        p[i] = cv::saturate_cast<uchar>((i - 127) * _contrast + 127.0);

    if (_lut.empty())
        _lut.create(256, 1, CV_8UC3);

    std::vector<int> from_to{0, 0, 0, 1, 0, 2};
    cv::mixChannels(lut, _lut, from_to);
}
