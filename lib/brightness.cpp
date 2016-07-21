#include <filatti/brightness.hpp>

#include <opencv2/imgproc.hpp>

using namespace filatti;

Brightness::Brightness() {
    _brightness = BRIGHTNESS_NONE;
}

Brightness::~Brightness() {
}

double Brightness::get_brightness() {
    return _brightness;
}

bool Brightness::set_brightness(double brightness) {
    if (_brightness < BRIGHTNESS_MIN || _brightness > BRIGHTNESS_MAX)
        return false;

    _brightness = brightness;
    build_lut();
    return true;
}

bool Brightness::apply(cv::Mat &src, cv::Mat &dst) {
    if (_brightness == BRIGHTNESS_NONE) {
        return false;
    } else {
        cv::Mat hls;
        cv::cvtColor(src, hls, cv::COLOR_BGR2HLS);

        std::vector<cv::Mat> channels(3);
        cv::split(hls, channels);

        cv::LUT(channels[1], _lut, channels[1]);

        cv::merge(channels, hls);

        cv::cvtColor(hls, dst, cv::COLOR_HLS2BGR);
        return true;
    }
}

void Brightness::build_lut() {
    if (_brightness == BRIGHTNESS_NONE) {
        if (!_lut.empty())
            _lut.release();
        return;
    }

    if (_lut.empty())
        _lut.create(256, 1, CV_8UC1);

    uchar *p = _lut.data;
    for (int i = 0; i < 256; ++i)
        p[i] = cv::saturate_cast<uchar>(i + (i * _brightness));
}
