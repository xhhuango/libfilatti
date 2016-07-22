#include <filatti/saturation.hpp>

#include <opencv2/imgproc.hpp>

using namespace filatti;

Saturation::Saturation() {
    _saturation = SATURATION_NONE;
}

Saturation::~Saturation() {
}

double Saturation::get_saturation() {
    return _saturation;
}

bool Saturation::set_saturation(double saturation) {
    if (!within(saturation, SATURATION_MIN, SATURATION_MAX))
        return false;

    _saturation = saturation;
    build_lut();
    return true;
}

bool Saturation::apply(cv::Mat& src, cv::Mat& dst) {
    if (_saturation == SATURATION_NONE) {
        return false;
    } else {
        cv::Mat hsv;
        cv::cvtColor(src, hsv, cv::COLOR_BGR2HSV);

        cv::Mat s;
        cv::extractChannel(hsv, s, 1);

        cv::LUT(s, _lut, s);

        std::vector<int> from_to{0, 1};
        cv::mixChannels(s, hsv, from_to);

        cv::cvtColor(hsv, dst, cv::COLOR_HSV2BGR);
        return true;
    }
}

void Saturation::build_lut() {
    if (_saturation == SATURATION_NONE) {
        if (!_lut.empty())
            _lut.release();
        return;
    }

    if (_lut.empty())
        _lut.create(256, 1, CV_8UC1);

    uchar* p = _lut.data;
    for (int i = 0; i < 256; ++i)
        p[i] = cv::saturate_cast<uchar>(i + (i * _saturation));
}
