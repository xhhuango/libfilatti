#include <filatti/color_balance.hpp>
#include <filatti/contract.hpp>

#include <opencv2/opencv.hpp>

using namespace filatti;

ColorBalance::ColorBalance() : Dirty(true), _is_add_sub_lut_built(false) {
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
                if (!_is_add_sub_lut_built) {
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
        create_lut(_shadows_add_lut, TONE_LUT_MAT_TYPE);
    }
    if (_shadows_sub_lut.empty()) {
        create_lut(_shadows_sub_lut, TONE_LUT_MAT_TYPE);
    }
    if (_midtones_add_lut.empty()) {
        create_lut(_midtones_add_lut, TONE_LUT_MAT_TYPE);
    }
    if (_midtones_sub_lut.empty()) {
        create_lut(_midtones_sub_lut, TONE_LUT_MAT_TYPE);
    }
    if (_highlights_add_lut.empty()) {
        create_lut(_highlights_add_lut, TONE_LUT_MAT_TYPE);
    }
    if (_highlights_sub_lut.empty()) {
        create_lut(_highlights_sub_lut, TONE_LUT_MAT_TYPE);
    }

    ToneLutType* p_shadows_add_lut = _shadows_add_lut.ptr<ToneLutType>(0);
    ToneLutType* p_shadows_sub_lut = _shadows_sub_lut.ptr<ToneLutType>(0);
    ToneLutType* p_highlights_add_lut = _highlights_add_lut.ptr<ToneLutType>(0);
    ToneLutType* p_highlights_sub_lut = _highlights_sub_lut.ptr<ToneLutType>(0);
    ToneLutType* p_midtones_add_lut = _midtones_add_lut.ptr<ToneLutType>(0);
    ToneLutType* p_midtones_sub_lut = _midtones_sub_lut.ptr<ToneLutType>(0);
    for (int i = 0, j = _shadows_add_lut.rows, k = j - 1; i < j; ++i, --k) {
        ToneLutType sh = ToneLutType(1.075) - ToneLutType(1) / (ToneLutType(i) / ToneLutType(16) + ToneLutType(1));

        ToneLutType m = (ToneLutType(i) - ToneLutType(127)) / ToneLutType(127);
        m = ToneLutType(0.667) * (ToneLutType(1) - m * m);

        p_shadows_add_lut[i] = sh;
        p_shadows_sub_lut[j] = sh;
        p_highlights_add_lut[k] = sh;
        p_highlights_sub_lut[k] = sh;
        p_midtones_add_lut[i] = m;
        p_midtones_sub_lut[i] = m;
    }

    _is_add_sub_lut_built = true;
}

void ColorBalance::build_lut() {
    if (_lut.empty()) {
        create_lut(_lut, CV_8UC3);
    }

    int blue_yellow[TONES];
    ToneLutType* p_blue_yellow_luts[TONES];
    compute(_blue_yellow, _green_magenta, _red_cyan, blue_yellow);
    p_blue_yellow_luts[SHADOWS] = blue_yellow[SHADOWS] > 0
                                  ? _shadows_add_lut.ptr<ToneLutType>(0)
                                  : _shadows_sub_lut.ptr<ToneLutType>(0);
    p_blue_yellow_luts[MIDTONES] = blue_yellow[MIDTONES] > 0
                                   ? _midtones_add_lut.ptr<ToneLutType>(0)
                                   : _midtones_sub_lut.ptr<ToneLutType>(0);
    p_blue_yellow_luts[HIGHLIGHTS] = blue_yellow[HIGHLIGHTS] > 0
                                     ? _highlights_add_lut.ptr<ToneLutType>(0)
                                     : _highlights_sub_lut.ptr<ToneLutType>(0);

    int green_magenta[TONES];
    compute(_green_magenta, _blue_yellow, _red_cyan, green_magenta);
    ToneLutType* p_green_magenta_luts[TONES];
    p_green_magenta_luts[SHADOWS] = green_magenta[SHADOWS] > 0
                                    ? _shadows_add_lut.ptr<ToneLutType>(0)
                                    : _shadows_sub_lut.ptr<ToneLutType>(0);
    p_green_magenta_luts[MIDTONES] = green_magenta[MIDTONES] > 0
                                     ? _midtones_add_lut.ptr<ToneLutType>(0)
                                     : _midtones_sub_lut.ptr<ToneLutType>(0);
    p_green_magenta_luts[HIGHLIGHTS] = green_magenta[HIGHLIGHTS] > 0
                                       ? _highlights_add_lut.ptr<ToneLutType>(0)
                                       : _highlights_sub_lut.ptr<ToneLutType>(0);

    int red_cyan[TONES];
    compute(_red_cyan, _blue_yellow, _green_magenta, red_cyan);
    ToneLutType* p_red_cyan_luts[TONES];
    p_red_cyan_luts[SHADOWS] = red_cyan[SHADOWS] > 0
                               ? _shadows_add_lut.ptr<ToneLutType>(0)
                               : _shadows_sub_lut.ptr<ToneLutType>(0);
    p_red_cyan_luts[MIDTONES] = red_cyan[MIDTONES] > 0
                                ? _midtones_add_lut.ptr<ToneLutType>(0)
                                : _midtones_sub_lut.ptr<ToneLutType>(0);
    p_red_cyan_luts[HIGHLIGHTS] = red_cyan[HIGHLIGHTS] > 0
                                  ? _highlights_add_lut.ptr<ToneLutType>(0)
                                  : _highlights_sub_lut.ptr<ToneLutType>(0);

    uchar* p_lut = _lut.ptr<uchar>(0);
    for (int i = 0, j = _lut.rows; i < j; ++i) {
        uchar b = (uchar) i;
        uchar g = (uchar) i;
        uchar r = (uchar) i;

        for (int tone = 0; tone < TONES; ++tone) {
            b = cv::saturate_cast<uchar>(ToneLutType(b) + ToneLutType(blue_yellow[tone]) * p_blue_yellow_luts[tone][b]);
            g = cv::saturate_cast<uchar>(ToneLutType(g) + ToneLutType(green_magenta[tone])* p_green_magenta_luts[tone][g]);
            r = cv::saturate_cast<uchar>(ToneLutType(r) + ToneLutType(red_cyan[tone]) * p_red_cyan_luts[tone][r]);
        }

        *p_lut++ = b;
        *p_lut++ = g;
        *p_lut++ = r;
    }
}

void ColorBalance::compute(int add[TONES], int sub1[TONES], int sub2[TONES], int res[TONES]) {
    res[SHADOWS] = add[SHADOWS] - sub1[SHADOWS] - sub2[SHADOWS];
    res[MIDTONES] = add[MIDTONES] - sub1[MIDTONES] - sub2[MIDTONES];
    res[HIGHLIGHTS] = add[HIGHLIGHTS] - sub1[HIGHLIGHTS] - sub2[HIGHLIGHTS];
}
