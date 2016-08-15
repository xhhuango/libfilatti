#include <filatti/hsv.hpp>

#include <opencv2/imgproc.hpp>

using namespace filatti;

Hsv::Hsv() {
    _brightness = BRIGHTNESS_NONE;
    _saturation = SATURATION_NONE;
}

Hsv::~Hsv() {
}

double Hsv::get_brightness() {
    return _brightness;
}

bool Hsv::set_brightness(double brightness) {
    if (!within(brightness, BRIGHTNESS_MIN, BRIGHTNESS_MAX))
        return false;

    _brightness = brightness;
    build_lut();
    return true;
}

double Hsv::get_saturation() {
    return _saturation;
}

bool Hsv::set_saturation(double saturation) {
    if (!within(saturation, SATURATION_MIN, SATURATION_MAX))
        return false;

    _saturation = saturation;
    build_lut();
    return true;
}

bool Hsv::has_effect() {
    return _brightness != BRIGHTNESS_NONE || _saturation != SATURATION_NONE;
}

bool Hsv::apply(const cv::Mat& src, cv::Mat& dst) {
    if (!has_effect()) {
        return false;
    } else {
        cv::Mat hls;
        cv::cvtColor(src, hls, cv::COLOR_BGR2HSV);

        cv::LUT(hls, _lut, hls);

        cv::cvtColor(hls, dst, cv::COLOR_HSV2BGR);
        return true;
    }
}

void Hsv::build_lut() {
    if (!has_effect()) {
        if (!_lut.empty()) {
            _lut.release();
        }
        return;;
    }

    if (_lut.empty()) {
        _lut.create(256, 1, CV_8UC3);
    }

    for (int i = 0; i < 256; ++i) {
        _lut.at<cv::Vec3b>(i, 0) = cv::Vec3b{calculate_h(i), calculate_s(i), calculate_v(i)};
    }
}

inline uchar Hsv::calculate_h(int h) {
    return cv::saturate_cast<uchar>(h);
}

inline uchar Hsv::calculate_s(int s) {
    return cv::saturate_cast<uchar>(s + (s * _saturation));
}

inline uchar Hsv::calculate_v(int v) {
    return cv::saturate_cast<uchar>(v + (v * _brightness));
}
