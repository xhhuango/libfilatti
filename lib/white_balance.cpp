#include <filatti/white_balance.hpp>
#include <filatti/contract.hpp>

#include <opencv2/opencv.hpp>

using namespace filatti;

WhiteBalance::WhiteBalance() : _percent(PERCENT_NONE) {
}

WhiteBalance::~WhiteBalance() {
}

bool WhiteBalance::has_effect() const noexcept {
    return _percent != PERCENT_NONE;
}

WhiteBalance::Type WhiteBalance::get_percent() const noexcept {
    return _percent;
}

void WhiteBalance::set_percent(Type percent) {
    PRECONDITION(percent >= PERCENT_MIN && percent <= PERCENT_MAX, "Percent is out of range");
    synchronize([=] {
        _percent = percent;
    });
}

bool WhiteBalance::apply(const cv::Mat& src, cv::Mat& dst) {
    if (!has_effect()) {
        return false;
    } else {
        synchronize([this, &src] {
            build_lut(src);
        });
        cv::LUT(src, _lut, dst);
        return true;
    }
}

void calculate_histogram(const cv::Mat& src, cv::MatND& hist) noexcept {
    int hist_size = 256;
    float range[] = {0, 256};
    const float* hist_ranges = {range};
    int channels[] = {0, 1, 2};
    cv::calcHist(&src, 1, channels, cv::Mat(), hist, 1, &hist_size, &hist_ranges, true, false);
}

std::vector<uchar> calculate_boundary(const cv::MatND& hist, unsigned int channel, float threshold) noexcept {
    std::vector<uchar> boundaries = {0, 255};

    float downer_sum = 0;
    for (int i = 0; i < 256; ++i) {
        downer_sum += hist.at<cv::Vec3f>(i, 0)[channel];
        if (downer_sum >= threshold) {
            boundaries[0] = uchar(i);
            break;
        }
    }

    float upper_sum = 0;
    for (int i = 255; i >= 0; --i) {
        upper_sum += hist.at<cv::Vec3f>(i, 0)[channel];
        if (upper_sum >= threshold) {
            boundaries[1] = uchar(i);
            break;
        }
    }

    return boundaries;
}

void WhiteBalance::build_lut(const cv::Mat& src) {
    if (_lut.empty()) {
        create_lut(_lut, CV_8UC3);
    }

    cv::MatND hist;
    calculate_histogram(src, hist);

    Type threshold = Type(src.rows * src.cols) * _percent;
    auto blue_boundary = calculate_boundary(hist, 0, float(threshold));
    auto green_boundary = calculate_boundary(hist, 1, float(threshold));
    auto red_boundary = calculate_boundary(hist, 2, float(threshold));

    Type blue_diff = blue_boundary[1] - blue_boundary[0];
    Type green_diff = green_boundary[1] - green_boundary[0];
    Type red_diff = red_boundary[1] - red_boundary[0];

    uchar* p_lut = _lut.ptr<uchar>(0);
    for (int i = 0, j = _lut.rows; i < j; ++i) {
        *p_lut++ = cv::saturate_cast<uchar>(Type(255) * ((Type(i) - Type(blue_boundary[0])) / blue_diff));
        *p_lut++ = cv::saturate_cast<uchar>(Type(255) * ((Type(i) - Type(green_boundary[0])) / green_diff));
        *p_lut++ = cv::saturate_cast<uchar>(Type(255) * ((Type(i) - Type(red_boundary[0])) / red_diff));
    }
}
