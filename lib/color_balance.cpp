#include <filatti/color_balance.hpp>
#include <filatti/contract.hpp>

using namespace filatti;

ColorBalance::ColorBalance() : Dirty(true), _is_add_sub_built(false) {
    for (int i = 0; i < TONES; ++i) {
        _red_cyan[i] = VALUE_NONE;
        _green_magenta[i] = VALUE_NONE;
        _blue_yellow[i] = VALUE_NONE;
    }
}

ColorBalance::~ColorBalance() {
}

bool ColorBalance::has_effect() const noexcept {
    for (int i = 0; i < TONES; ++i) {
        if (_red_cyan[i] != VALUE_NONE || _green_magenta[i] != VALUE_NONE || _blue_yellow[i] != VALUE_NONE) {
            return true;
        }
    }
    return false;
}

int ColorBalance::get_red_cyan(Tone tone) const noexcept {
    return _red_cyan[static_cast<ToneRawType>(tone)];
}

void ColorBalance::set_red_cyan(Tone tone, int value) {
    PRECONDITION(value >= VALUE_MIN && value <= VALUE_MAX, "Red/Cyan is out of range");
    synchronize([=] {
        _red_cyan[static_cast<ToneRawType>(tone)] = value;
        make_dirty();
    });
}

int ColorBalance::get_green_magenta(Tone tone) const noexcept {
    return _green_magenta[static_cast<ToneRawType>(tone)];
}

void ColorBalance::set_green_magenta(Tone tone, int value) {
    PRECONDITION(value >= VALUE_MIN && value <= VALUE_MAX, "Green/Magenta is out of range");
    synchronize([=] {
        _green_magenta[static_cast<ToneRawType>(tone)] = value;
        make_dirty();
    });
}

int ColorBalance::get_blue_yellow(Tone tone) const noexcept {
    return _blue_yellow[static_cast<ToneRawType>(tone)];
}

void ColorBalance::set_blue_yellow(Tone tone, int value) {
    PRECONDITION(value >= VALUE_MIN && value <= VALUE_MAX, "Blue/Yellow is out of range");
    synchronize([=] {
        _blue_yellow[static_cast<ToneRawType>(tone)] = value;
        make_dirty();
    });
}

bool ColorBalance::apply(const cv::Mat& src, cv::Mat& dst) {
    if (!has_effect()) {
        return false;
    } else {
        synchronize([this] {
            if (make_clean_if_dirty()) {
                if (!_is_add_sub_built) {
                    build_add_sub_luts();
                }
                build_lut();
            }
        });

        cv::LUT(src, _lut, dst);
        return true;
    }
}

void ColorBalance::build_add_sub_luts() {
    if (_shadows_add_lut.empty()) {
        _shadows_add_lut.create(256, 1, CV_32FC1);
    }
    if (_shadows_sub_lut.empty()) {
        _shadows_sub_lut.create(256, 1, CV_32FC1);
    }
    if (_midtones_add_lut.empty()) {
        _midtones_add_lut.create(256, 1, CV_32FC1);
    }
    if (_midtones_sub_lut.empty()) {
        _midtones_sub_lut.create(256, 1, CV_32FC1);
    }
    if (_highlights_add_lut.empty()) {
        _highlights_add_lut.create(256, 1, CV_32FC1);
    }
    if (_highlights_sub_lut.empty()) {
        _highlights_sub_lut.create(256, 1, CV_32FC1);
    }

    for (int i = 0; i < 256; ++i) {
        float sh = 1.075f - 1.0f / ((float) i / 16.0f + 1.0f);

        float m = ((float) i - 127.0f) / 127.0f;
        m = 0.667f * (1.0f - m * m);

        _shadows_add_lut.at<float>(i, 0) = sh;
        _shadows_sub_lut.at<float>(255 - i, 0) = sh;
        _highlights_add_lut.at<float>(255 - i, 0) = sh;
        _highlights_sub_lut.at<float>(i, 0) = sh;
        _midtones_add_lut.at<float>(i, 0) = m;
        _midtones_sub_lut.at<float>(i, 0) = m;
    }

    _is_add_sub_built = true;
}

void ColorBalance::build_lut() {
    if (_lut.empty()) {
        _lut.create(256, 1, CV_8UC3);
        PRECONDITION(_lut.isContinuous(), "LUT is not continuous");
    }

    cv::Mat* blue_yellow_transfers[TONES];
    blue_yellow_transfers[SHADOWS] = _blue_yellow[SHADOWS] > 0 ? &_shadows_add_lut : &_shadows_sub_lut;
    blue_yellow_transfers[MIDTONES] = _blue_yellow[MIDTONES] > 0 ? &_midtones_add_lut : &_midtones_sub_lut;
    blue_yellow_transfers[HIGHLIGHTS] = _blue_yellow[HIGHLIGHTS] > 0 ? &_highlights_add_lut : &_highlights_sub_lut;

    cv::Mat* green_magenta_transfers[TONES];
    green_magenta_transfers[SHADOWS] = _green_magenta[SHADOWS] > 0 ? &_shadows_add_lut : &_shadows_sub_lut;
    green_magenta_transfers[MIDTONES] = _green_magenta[MIDTONES] > 0 ? &_midtones_add_lut : &_midtones_sub_lut;
    green_magenta_transfers[HIGHLIGHTS] = _green_magenta[HIGHLIGHTS] > 0 ? &_highlights_add_lut : &_highlights_sub_lut;

    cv::Mat* red_cyan_transfers[TONES];
    red_cyan_transfers[SHADOWS] = _red_cyan[SHADOWS] > 0 ? &_shadows_add_lut : &_shadows_sub_lut;
    red_cyan_transfers[MIDTONES] = _red_cyan[MIDTONES] > 0 ? &_midtones_add_lut : &_midtones_sub_lut;
    red_cyan_transfers[HIGHLIGHTS] = _red_cyan[HIGHLIGHTS] > 0 ? &_highlights_add_lut : &_highlights_sub_lut;

    for (int i = 0; i < 256; ++i) {
        uchar b = (uchar) i;
        uchar g = (uchar) i;
        uchar r = (uchar) i;

        for (int tone = 0; tone < TONES; ++tone) {
            b = cv::saturate_cast<float>(
                    (float) b + (float) _blue_yellow[tone] * blue_yellow_transfers[tone]->at<float>(b, 0));
            g = cv::saturate_cast<float>(
                    (float) g + (float) _green_magenta[tone] * green_magenta_transfers[tone]->at<float>(g, 0));
            r = cv::saturate_cast<float>(
                    (float) r + (float) _red_cyan[tone] * red_cyan_transfers[tone]->at<float>(r, 0));
        }

        _lut.at<cv::Vec3b>(i, 0) = cv::Vec3b{b, g, r};
    }
}
