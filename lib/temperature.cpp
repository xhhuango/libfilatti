#include <filatti/temperature.hpp>

#include <opencv2/imgproc.hpp>

using namespace filatti;

Temperature::Temperature() {
    _kelvin = KELVIN_NONE;
    _strength = STRENGTH_NONE;
}

Temperature::~Temperature() {
}

unsigned int Temperature::get_kelvin() {
    return _kelvin;
}

bool Temperature::set_kelvin(unsigned int kelvin) {
    if (!within(kelvin, KELVIN_MIN, KELVIN_MAX))
        return false;

    _kelvin = kelvin;

    if (!_lut.empty())
        _lut.release();

    return true;
}

double Temperature::get_strength() {
    return _strength;
}

bool Temperature::set_strength(double strength) {
    if (!within(strength, STRENGTH_MIN, STRENGTH_MAX))
        return false;

    _strength = strength;

    if (!_lut.empty())
        _lut.release();

    return true;
}

bool Temperature::apply(cv::Mat& src, cv::Mat& dst) {
    if (_strength == STRENGTH_NONE || _kelvin == KELVIN_NONE) {
        return false;
    } else {
        if (_lut.empty())
            build_lut(kelvin_to_color(_kelvin));

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

cv::Vec3b Temperature::kelvin_to_color(unsigned int kelvin) {
    return cv::Vec3b{kelvin_to_b(kelvin), kelvin_to_g(kelvin), kelvin_to_r(kelvin)};
}

uchar Temperature::kelvin_to_b(unsigned int kelvin) {
    double k = kelvin / 100.0;

    if (k >= 66) {
        return 255;
    } else if (k <= 19) {
        return 0;
    } else {
        double blue = 138.5177312231 * std::log(k - 10) - 305.0447927307;
        return cv::saturate_cast<uchar>(blue);
    }
}

uchar Temperature::kelvin_to_g(unsigned int kelvin) {
    double green;
    double k = kelvin / 100.0;

    if (k <= 66) {
        green = 99.4708025861 * std::log(k) - 161.1195681661;
    } else {
        green = 288.1221695283 * std::pow(k - 60, -0.0755148492);
    }

    return cv::saturate_cast<uchar>(green);
}

uchar Temperature::kelvin_to_r(unsigned int kelvin) {
    double k = kelvin / 100.0;

    if (k <= 66) {
        return 255;
    } else {
        double red = 329.698727446 * std::pow(k - 60, -0.1332047592);
        return cv::saturate_cast<uchar>(red);
    }
}

void Temperature::build_lut(cv::Vec3b color) {
    _lut.create(256, 1, CV_8UC3);

    double kelvin_blue = color[0] * _strength;
    double kelvin_green = color[1] * _strength;
    double kelvin_red = color[2] * _strength;

    double src_strength = 1.0 - _strength;

    for (int i = 0; i < 256; ++i) {
        double i_strength = i * src_strength;
        uchar b = cv::saturate_cast<uchar>(i_strength + kelvin_blue);
        uchar g = cv::saturate_cast<uchar>(i_strength + kelvin_green);
        uchar r = cv::saturate_cast<uchar>(i_strength + kelvin_red);
        _lut.at<cv::Vec3b>(i, 0) = cv::Vec3b{b, g, r};
    }
}
