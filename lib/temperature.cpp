#include <filatti/temperature.hpp>
#include <filatti/contract.hpp>

#include <opencv2/imgproc.hpp>

using namespace filatti;

Temperature::Temperature() : Dirty(true), _temperature(TEMPERATURE_NONE) {
}

Temperature::~Temperature() {
}

bool Temperature::has_effect() const noexcept {
    return _temperature != TEMPERATURE_NONE;
}

double Temperature::get_temperature() const noexcept {
    return _temperature;
}

void Temperature::set_temperature(double temperature) {
    PRECONDITION(temperature >= TEMPERATURE_MIN && temperature <= TEMPERATURE_MAX, "Temperature is out of range");
    synchronize([=] {
        _temperature = temperature;
        make_dirty();
    });
}

bool Temperature::apply(const cv::Mat& src, cv::Mat& dst) {
    if (!has_effect()) {
        return false;
    } else {
        synchronize([this] {
            if (make_clean_if_dirty()) {
                build_lut();
            }
        });

        cv::LUT(src, _lut, dst);
        return true;
    }
}

void Temperature::build_lut() {
    if (_lut.empty()) {
        _lut.create(256, 1, CV_8UC3);
        PRECONDITION(_lut.isContinuous(), "Lut is not continuous");
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
