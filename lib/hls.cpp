#include <filatti/hls.hpp>

#include <opencv2/imgproc.hpp>

using namespace filatti;

Hls::Hls() {
    _hue = HUE_NONE;
    _lightness = LIGHTNESS_NONE;
    _saturation = SATURATION_NONE;
}

Hls::~Hls() {
}

int Hls::get_hue() {
    return _hue;
}

bool Hls::set_hue(int hue) {
    if (!within(hue, HUE_MIN, HUE_MAX))
        return false;

    _hue = hue;
    build_lut();
    return true;
}

double Hls::get_lightness() {
    return _lightness;
}

bool Hls::set_lightness(double lightness) {
    if (!within(lightness, LIGHTNESS_MIN, LIGHTNESS_MAX))
        return false;

    _lightness = lightness;
    build_lut();
    return true;
}

double Hls::get_saturation() {
    return _saturation;
}

bool Hls::set_saturation(double saturation) {
    if (!within(saturation, SATURATION_MIN, SATURATION_MAX))
        return false;

    _saturation = saturation;
    build_lut();
    return true;
}

bool Hls::has_effect() {
    return _hue != HUE_NONE || _lightness != LIGHTNESS_NONE || _saturation != SATURATION_NONE;
}

bool Hls::apply(const cv::Mat& src, cv::Mat& dst) {
    if (!has_effect()) {
        return false;
    } else {
        cv::Mat hls;
        cv::cvtColor(src, hls, cv::COLOR_BGR2HLS);

        cv::LUT(hls, _lut, hls);

        cv::cvtColor(hls, dst, cv::COLOR_HLS2BGR);
        return true;
    }
}

void Hls::build_lut() {
    if (!has_effect()) {
        if (!_lut.empty()) {
            _lut.release();
        }
        return;;
    }

    if (_lut.empty()) {
        _lut.create(256, 1, CV_8UC3);
    }

    int h_shift = _hue / 2;

    for (int i = 0; i < 256; ++i) {
        int h_int = i + h_shift;
        if (h_int > 180)
            h_int %= 180;
        else if (h_int < 0)
            h_int += 180;
        uchar h = cv::saturate_cast<uchar>(h_int);

        uchar l = cv::saturate_cast<uchar>(i + (i * _lightness));
        uchar s = cv::saturate_cast<uchar>(i + (i * _saturation));

        _lut.at<cv::Vec3b>(i, 0) = cv::Vec3b{h, l, s};
    }
}
