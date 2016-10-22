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

Temperature::Type Temperature::get_temperature() const noexcept {
    return _temperature;
}

void Temperature::set_temperature(Type temperature) {
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
        create_lut(_lut, CV_8UC3);
    }

    Type enhanced_blue = 0;
    Type enhanced_red = 0;
    Type src_strength;

    if (_temperature > 0) {
        enhanced_red = Type(255) * _temperature;
        src_strength = Type(1) - _temperature;
    } else {
        enhanced_blue = Type(255) * -_temperature;
        src_strength = Type(1) + _temperature;
    }

    uchar* p_lut = _lut.ptr<uchar>(0);
    for (int i = 0, j = _lut.rows; i < j; ++i) {
        Type i_strength = i * src_strength;
        *p_lut++ = cv::saturate_cast<uchar>(i_strength + enhanced_blue);
        *p_lut++ = cv::saturate_cast<uchar>(i);
        *p_lut++ = cv::saturate_cast<uchar>(i_strength + enhanced_red);
    }
}
