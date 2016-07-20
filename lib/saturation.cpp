#include <filatti/saturation.hpp>

#include <opencv2/imgproc.hpp>

using namespace filatti;

Saturation::Saturation() : _saturation{0} {
}

Saturation::~Saturation() {
}

double Saturation::get_saturation() {
    return _saturation;
}

bool Saturation::set_saturation(double saturation) {
    if (_saturation < SATURATION_MIN || _saturation > SATURATION_MAX)
        return false;

    _saturation = saturation;
    build_lut();
    return true;
}

bool Saturation::apply(cv::Mat &src, cv::Mat &dst) {
    if (_saturation == SATURATION_NONE) {
        return false;
    } else {
        cv::Mat hsv;
        cvtColor(src, hsv, cv::COLOR_RGB2HSV);

        std::vector<cv::Mat> channels(3);
        split(hsv, channels);

        LUT(channels[1], _lut, channels[1]);

        merge(channels, hsv);

        cvtColor(hsv, dst, cv::COLOR_HSV2RGB);
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

    uchar *p = _lut.data;
    for (int i = 0; i < 256; ++i)
        p[i] = cv::saturate_cast<uchar>(i + (i * _saturation));
}
