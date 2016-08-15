#include <filatti/hls.hpp>

#include <opencv2/imgproc.hpp>

using namespace filatti;

Hls::Hls() {
    _brightness = BRIGHTNESS_NONE;
    _saturation = SATURATION_NONE;
}

Hls::~Hls() {
}

double Hls::get_brightness() {
    return _brightness;
}

bool Hls::set_brightness(double brightness) {
    if (!within(brightness, BRIGHTNESS_MIN, BRIGHTNESS_MAX))
        return false;

    _brightness = brightness;
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
    return _brightness != BRIGHTNESS_NONE || _saturation != SATURATION_NONE;
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

    for (int i = 0; i < 256; ++i) {
        _lut.at<cv::Vec3b>(i, 0) = cv::Vec3b{calculate_h(i), calculate_l(i), calculate_s(i)};
    }
}

inline uchar Hls::calculate_h(int h) {
    return cv::saturate_cast<uchar>(h);
}

inline uchar Hls::calculate_l(int l) {
    return cv::saturate_cast<uchar>(l + (l * _brightness));
}

inline uchar Hls::calculate_s(int s) {
    return cv::saturate_cast<uchar>(s + (s * _saturation));
}
