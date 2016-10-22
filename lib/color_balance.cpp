#include <filatti/color_balance.hpp>
#include <filatti/contract.hpp>

#include <opencv2/imgproc.hpp>

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

    float* p_shadows_add_lut = _shadows_add_lut.ptr<float>(0);
    float* p_shadows_sub_lut = _shadows_sub_lut.ptr<float>(0);
    float* p_highlights_add_lut = _highlights_add_lut.ptr<float>(0);
    float* p_highlights_sub_lut = _highlights_sub_lut.ptr<float>(0);
    float* p_midtones_add_lut = _midtones_add_lut.ptr<float>(0);
    float* p_midtones_sub_lut = _midtones_sub_lut.ptr<float>(0);
    for (int i = 0, j = 255; i < 256; ++i, --j) {
        float sh = 1.075f - 1.0f / ((float) i / 16.0f + 1.0f);

        float m = ((float) i - 127.0f) / 127.0f;
        m = 0.667f * (1.0f - m * m);

        p_shadows_add_lut[i] = sh;
        p_shadows_sub_lut[j] = sh;
        p_highlights_add_lut[j] = sh;
        p_highlights_sub_lut[i] = sh;
        p_midtones_add_lut[i] = m;
        p_midtones_sub_lut[i] = m;
    }

    _is_add_sub_lut_built = true;
}

void ColorBalance::build_lut() {
    if (_lut.empty()) {
        _lut.create(256, 1, CV_8UC3);
    }

    int blue_yellow[TONES];
    float* p_blue_yellow_luts[TONES];
    compute(_blue_yellow, _green_magenta, _red_cyan, blue_yellow);
    p_blue_yellow_luts[SHADOWS] = blue_yellow[SHADOWS] > 0
                                  ? _shadows_add_lut.ptr<float>(0)
                                  : _shadows_sub_lut.ptr<float>(0);
    p_blue_yellow_luts[MIDTONES] = blue_yellow[MIDTONES] > 0
                                   ? _midtones_add_lut.ptr<float>(0)
                                   : _midtones_sub_lut.ptr<float>(0);
    p_blue_yellow_luts[HIGHLIGHTS] = blue_yellow[HIGHLIGHTS] > 0
                                     ? _highlights_add_lut.ptr<float>(0)
                                     : _highlights_sub_lut.ptr<float>(0);

    int green_magenta[TONES];
    compute(_green_magenta, _blue_yellow, _red_cyan, green_magenta);
    float* p_green_magenta_luts[TONES];
    p_green_magenta_luts[SHADOWS] = green_magenta[SHADOWS] > 0
                                    ? _shadows_add_lut.ptr<float>(0)
                                    : _shadows_sub_lut.ptr<float>(0);
    p_green_magenta_luts[MIDTONES] = green_magenta[MIDTONES] > 0
                                     ? _midtones_add_lut.ptr<float>(0)
                                     : _midtones_sub_lut.ptr<float>(0);
    p_green_magenta_luts[HIGHLIGHTS] = green_magenta[HIGHLIGHTS] > 0
                                       ? _highlights_add_lut.ptr<float>(0)
                                       : _highlights_sub_lut.ptr<float>(0);

    int red_cyan[TONES];
    compute(_red_cyan, _blue_yellow, _green_magenta, red_cyan);
    float* p_red_cyan_luts[TONES];
    p_red_cyan_luts[SHADOWS] = red_cyan[SHADOWS] > 0
                               ? _shadows_add_lut.ptr<float>(0)
                               : _shadows_sub_lut.ptr<float>(0);
    p_red_cyan_luts[MIDTONES] = red_cyan[MIDTONES] > 0
                                ? _midtones_add_lut.ptr<float>(0)
                                : _midtones_sub_lut.ptr<float>(0);
    p_red_cyan_luts[HIGHLIGHTS] = red_cyan[HIGHLIGHTS] > 0
                                  ? _highlights_add_lut.ptr<float>(0)
                                  : _highlights_sub_lut.ptr<float>(0);

    uchar* p_lut = _lut.ptr<uchar>(0);
    for (int i = 0; i < 256; ++i) {
        uchar b = (uchar) i;
        uchar g = (uchar) i;
        uchar r = (uchar) i;

        for (int tone = 0; tone < TONES; ++tone) {
            b = cv::saturate_cast<uchar>(float(b) + float(blue_yellow[tone]) * p_blue_yellow_luts[tone][b]);
            g = cv::saturate_cast<uchar>(float(g) + float(green_magenta[tone])* p_green_magenta_luts[tone][g]);
            r = cv::saturate_cast<uchar>(float(r) + float(red_cyan[tone]) * p_red_cyan_luts[tone][r]);
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
