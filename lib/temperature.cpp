#include <filatti/temperature.hpp>

#include <opencv2/imgproc.hpp>

using namespace filatti;

Temperature::Temperature() {
    _temperature = TEMPERATURE_NONE;
}

Temperature::~Temperature() {
}

bool Temperature::has_effect() const {
    return _temperature != TEMPERATURE_NONE;
}

double Temperature::get_temperature() const {
    return _temperature;
}

bool Temperature::set_temperature(double temperature) {
    if (!within(temperature, TEMPERATURE_MIN, TEMPERATURE_MAX)) {
        return false;
    }

    _temperature = temperature;
    release_lut();
    return true;
}

bool Temperature::apply(const cv::Mat& src, cv::Mat& dst) {
    if (!has_effect()) {
        return false;
    } else {
        if (_lut.empty()) {
            build_lut();
        }

        cv::Mat orig_y;
        cv::Mat ycrcb;

        cv::cvtColor(src, ycrcb, cv::COLOR_BGR2YCrCb);
        cv::extractChannel(ycrcb, orig_y, 0);
        ycrcb.release();

        cv::LUT(src, _lut, dst);

        cv::cvtColor(dst, ycrcb, cv::COLOR_BGR2YCrCb);
        cv::mixChannels(orig_y, ycrcb, std::vector<int>(0, 0));
        cv::cvtColor(ycrcb, dst, cv::COLOR_YCrCb2BGR);

        return true;
    }
}

void Temperature::release_lut() {
    if (!_lut.empty()) {
        _lut.release();
    }
}

void Temperature::build_lut() {
    if (_lut.empty()) {
        _lut.create(256, 1, CV_8UC3);
    }

    double enhanced_blue = 0;
    double enhanced_red = 0;
    double src_strength;

    if (_temperature > 0) {
        enhanced_red = 255 * _temperature;
        src_strength = 1 - _temperature;
    } else {
        enhanced_blue = 255 * -_temperature;
        src_strength = 1 + _temperature;
    }

    for (int i = 0; i < 256; ++i) {
        double i_strength = i * src_strength;
        uchar blue = cv::saturate_cast<uchar>(i_strength + enhanced_blue);
        uchar green = cv::saturate_cast<uchar>(i);
        uchar red = cv::saturate_cast<uchar>(i_strength + enhanced_red);
        _lut.at<cv::Vec3b>(i, 0) = cv::Vec3b{blue, green, red};
    }
}
