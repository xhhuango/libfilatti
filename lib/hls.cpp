#include <filatti/hls.hpp>
#include <filatti/contract.hpp>

#include <opencv2/imgproc.hpp>

using namespace filatti;

Hls::Hls() : Dirty(true), _hue(HUE_NONE), _lightness(LIGHTNESS_NONE), _saturation(SATURATION_NONE) {
}

Hls::~Hls() {
}

bool Hls::has_effect() const noexcept {
    return _hue != HUE_NONE || _lightness != LIGHTNESS_NONE || _saturation != SATURATION_NONE;
}

int Hls::get_hue() const noexcept {
    return _hue;
}

void Hls::set_hue(int hue) {
    PRECONDITION(hue >= HUE_MIN && hue <= HUE_MAX, "Hue is out of range");
    synchronize([=] {
        _hue = hue;
        make_dirty();
    });
}

Hls::Type Hls::get_lightness() const noexcept {
    return _lightness;
}

void Hls::set_lightness(Type lightness) {
    PRECONDITION(lightness >= LIGHTNESS_MIN && lightness <= LIGHTNESS_MAX, "Lightness is out of range");
    synchronize([=] {
        _lightness = lightness;
        make_dirty();
    });
}

Hls::Type Hls::get_saturation() const noexcept {
    return _saturation;
}

void Hls::set_saturation(Type saturation) {
    PRECONDITION(saturation >= SATURATION_MIN && saturation <= SATURATION_MAX, "Saturation is out of range");
    synchronize([=] {
        _saturation = saturation;
        make_dirty();
    });
}

bool Hls::apply(const cv::Mat& src, cv::Mat& dst) {
    if (!has_effect()) {
        return false;
    } else {
        synchronize([this] {
            if (make_clean_if_dirty()) {
                build_lut();
            }
        });

        cv::Mat hls;
        cv::cvtColor(src, hls, cv::COLOR_BGR2HLS);
        cv::LUT(hls, _lut, hls);
        cv::cvtColor(hls, dst, cv::COLOR_HLS2BGR);
        return true;
    }
}

void Hls::build_lut() {
    if (_lut.empty()) {
        create_lut(_lut, CV_8UC3);
    }

    int h_shift = _hue / 2;

    uchar* p_lut = _lut.ptr<uchar>(0);
    for (int i = 0, j = _lut.rows; i < j; ++i) {
        int h_int = i + h_shift;
        if (h_int > 180) {
            h_int %= 180;
        } else if (h_int < 0) {
            h_int += 180;
        }

        *p_lut++ = cv::saturate_cast<uchar>(h_int);
        *p_lut++ = cv::saturate_cast<uchar>(i + (i * _lightness));
        *p_lut++ = cv::saturate_cast<uchar>(i + (i * _saturation));
    }
}
