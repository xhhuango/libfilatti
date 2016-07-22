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
    if (!within(brightness, BRIGHTNESS_MIN, BRIGHTNESS_MAX))
        return false;

    _brightness = brightness;
    build_lut();
    return true;
}

bool Brightness::apply(cv::Mat& src, cv::Mat& dst) {
    if (_brightness == BRIGHTNESS_NONE) {
        return false;
    } else {
        cv::Mat hls;
        cv::cvtColor(src, hls, cv::COLOR_BGR2HLS);

        cv::Mat l;
        cv::extractChannel(hls, l, 1);

        cv::LUT(l, _lut, l);

        std::vector<int> from_to{0, 1};
        cv::mixChannels(l, hls, from_to);

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

    uchar* p = _lut.data;
    for (int i = 0; i < 256; ++i)
        p[i] = cv::saturate_cast<uchar>(i + (i * _brightness));
}
