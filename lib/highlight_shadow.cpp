#include <filatti/highlight_shadow.hpp>
#include <filatti/contract.hpp>

#include <opencv2/opencv.hpp>

using namespace filatti;

HighlightShadow::HighlightShadow() : Dirty(true),
                                     _shadow_amount(AMOUNT_NONE),
                                     _shadow_tone_width(TONE_WIDTH_MAX),
                                     _highlight_amount(AMOUNT_NONE),
                                     _highlight_tone_width(TONE_WIDTH_MAX) {
}

HighlightShadow::~HighlightShadow() {
}

bool HighlightShadow::has_effect() const noexcept {
    return _shadow_amount != AMOUNT_NONE || _highlight_amount != AMOUNT_NONE;
}

double HighlightShadow::get_amount(const Tone tone) const {
    PRECONDITION(tone == Tone::HIGHLIGHTS || tone == Tone::SHADOWS, "Tone is not supported");
    return tone == Tone::HIGHLIGHTS ? _highlight_amount : _shadow_amount;
}

void HighlightShadow::set_amount(Tone tone, double amount) {
    PRECONDITION(tone == Tone::HIGHLIGHTS || tone == Tone::SHADOWS, "Tone is not supported");
    PRECONDITION(amount >= AMOUNT_MIN && amount <= AMOUNT_MAX, "Amount is out of range");

    synchronize([=] {
        if (tone == Tone::HIGHLIGHTS) {
            _highlight_amount = amount;
        } else {
            _shadow_amount = amount;
        }
        make_dirty();
    });
}

double HighlightShadow::get_tone_width(Tone tone) const {
    PRECONDITION(tone == Tone::HIGHLIGHTS || tone == Tone::SHADOWS, "Tone is not supported");
    return tone == Tone::HIGHLIGHTS ? _highlight_tone_width : _shadow_tone_width;
}

void HighlightShadow::set_tone_width(Tone tone, double width) {
    PRECONDITION(tone == Tone::HIGHLIGHTS || tone == Tone::SHADOWS, "Tone is not supported");
    PRECONDITION(width >= TONE_WIDTH_MIN && width <= TONE_WIDTH_MAX, "Tone width is out of range");

    synchronize([=] {
        if (tone == Tone::HIGHLIGHTS) {
            _highlight_tone_width = width;
        } else {
            _shadow_tone_width = width;
        }
        make_dirty();
    });
}

bool HighlightShadow::apply(const cv::Mat& src, cv::Mat& dst) {
    if (!has_effect()) {
        return false;
    } else {
        synchronize([this, &src] {
            if (make_clean_if_dirty()) {
                build_lut();
            }
        });

        cv::Mat ycrcb;
        cv::cvtColor(src, ycrcb, cv::COLOR_BGR2YCrCb);

        cv::Mat y;
        cv::extractChannel(ycrcb, y, 0);
        cv::LUT(y, _lut, y);
        cv::mixChannels(y, ycrcb, std::vector<int>{0, 0});

        cv::cvtColor(ycrcb, dst, cv::COLOR_YCrCb2BGR);

        return true;
    }
}

void HighlightShadow::build_lut() {
    if (_lut.empty()) {
        _lut.create(256, 1, CV_8UC1);
        PRECONDITION(_lut.isContinuous(), "LUT is not continuous");
    }

    double shadow_cutoff = 255 * _shadow_tone_width;
    double highlight_cutoff = 255 - 255 * _highlight_tone_width;

    for (int i = 0; i < 256; ++i) {
        double value = i;

        if (value < shadow_cutoff) {
            value = (shadow_cutoff - value) * _shadow_amount + value;
        } else if (value > highlight_cutoff) {
            value = (value - highlight_cutoff) * _highlight_amount + value;
        }

        _lut.at<uchar>(i, 0) = cv::saturate_cast<uchar>(value);
    }
}
