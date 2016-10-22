#include <filatti/curves.hpp>
#include <filatti/contract.hpp>

#include "interpolator/spline.hpp"
#include "interpolator/linear.hpp"

#include <memory>

using namespace filatti;

using InterpolatorType = float;

inline interpolator::Interpolator<InterpolatorType>*
new_interpolator(const std::pair<std::vector<uchar>, std::vector<uchar>>& points) {
    if (points.first.size() > 2) {
        return new interpolator::Spline<InterpolatorType>(
                std::vector<InterpolatorType>(points.first.begin(), points.first.end()),
                std::vector<InterpolatorType>(points.second.begin(), points.second.end()));
    } else {
        return new interpolator::Linear<InterpolatorType>(
                std::vector<InterpolatorType>(points.first.begin(), points.first.end()),
                std::vector<InterpolatorType>(points.second.begin(), points.second.end()));
    }
}

inline std::vector<uchar> get_curves(const std::pair<std::vector<uchar>, std::vector<uchar>>& points) {
    std::unique_ptr<interpolator::Interpolator<InterpolatorType>> interpolator(new_interpolator(points));
    std::vector<uchar> curves(256);
    for (int i = 0, j = (int) curves.size(); i < j; ++i) {
        curves[i] = cv::saturate_cast<uchar>((*interpolator)[i]);
    }
    return curves;
}

Curves::Curves() : Dirty(true) {
    _value_points = std::make_pair(POINTS_NONE, POINTS_NONE);
    _blue_points = std::make_pair(POINTS_NONE, POINTS_NONE);
    _green_points = std::make_pair(POINTS_NONE, POINTS_NONE);
    _red_points = std::make_pair(POINTS_NONE, POINTS_NONE);
}

Curves::~Curves() {
}

bool Curves::has_effect() const noexcept {
    return !(_value_points.first == POINTS_NONE && _value_points.second == POINTS_NONE
             && _blue_points.first == POINTS_NONE && _blue_points.second == POINTS_NONE
             && _green_points.first == POINTS_NONE && _green_points.second == POINTS_NONE
             && _red_points.first == POINTS_NONE && _red_points.second == POINTS_NONE);
}

inline void Curves::check_input_points(const std::vector<uchar>& from, const std::vector<uchar>& to) const {
    auto from_size = from.size();
    PRECONDITION(from_size >= POINTS_SIZE_MIN && from_size <= POINTS_SIZE_MAX, "Size of points are out of range");
    PRECONDITION(from_size == to.size(), "Size of 'from' and 'to' points are not the same");
    PRECONDITION(from.front() == POINTS_NONE.front(), "The first point in 'from' points is illegal");
    PRECONDITION(from.back() == POINTS_NONE.back(), "The last point in 'from' points is illegal");
}

void Curves::set_value_points(const std::vector<uchar>& from, const std::vector<uchar>& to) {
    check_input_points(from, to);
    synchronize([&] {
        _value_points = std::make_pair(from, to);
        make_dirty();
    });
}

std::pair<std::vector<uchar>, std::vector<uchar>> Curves::get_value_points() const noexcept {
    return _value_points;
}

std::vector<uchar> Curves::get_value_curves() const noexcept {
    return get_curves(_value_points);
}

void Curves::set_blue_points(const std::vector<uchar>& from, const std::vector<uchar>& to) {
    check_input_points(from, to);
    synchronize([&] {
        _blue_points = std::make_pair(from, to);
        make_dirty();
    });
}

std::pair<std::vector<uchar>, std::vector<uchar>> Curves::get_blue_points() const noexcept {
    return _blue_points;
}

std::vector<uchar> Curves::get_blue_curves() const noexcept {
    return get_curves(_blue_points);
}

void Curves::set_green_points(const std::vector<uchar>& from, const std::vector<uchar>& to) {
    check_input_points(from, to);
    synchronize([&] {
        _green_points = std::make_pair(from, to);
        make_dirty();
    });
}

std::pair<std::vector<uchar>, std::vector<uchar>> Curves::get_green_points() const noexcept {
    return _green_points;
}

std::vector<uchar> Curves::get_green_curves() const noexcept {
    return get_curves(_green_points);
}

void Curves::set_red_points(const std::vector<uchar>& from, const std::vector<uchar>& to) {
    check_input_points(from, to);
    synchronize([&] {
        _red_points = std::make_pair(from, to);
    });
    make_dirty();
}

std::pair<std::vector<uchar>, std::vector<uchar>> Curves::get_red_points() const noexcept {
    return _red_points;
}

std::vector<uchar> Curves::get_red_curves() const noexcept {
    return get_curves(_red_points);
}

bool Curves::apply(const cv::Mat& src, cv::Mat& dst) {
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

void Curves::build_lut() {
    if (_lut.empty()) {
        create_lut(_lut, CV_8UC3);
    }

    std::unique_ptr<interpolator::Interpolator<InterpolatorType>> value_interpolator(new_interpolator(_value_points));
    std::unique_ptr<interpolator::Interpolator<InterpolatorType>> blue_interpolator(new_interpolator(_blue_points));
    std::unique_ptr<interpolator::Interpolator<InterpolatorType>> green_interpolator(new_interpolator(_green_points));
    std::unique_ptr<interpolator::Interpolator<InterpolatorType>> red_interpolator(new_interpolator(_red_points));

    uchar* p_lut = _lut.ptr<uchar>(0);
    for (int i = 0, j = _lut.rows; i < j; ++i) {
        auto value = (*value_interpolator)[i];
        *p_lut++ = cv::saturate_cast<uchar>((*blue_interpolator)[value]);
        *p_lut++ = cv::saturate_cast<uchar>((*green_interpolator)[value]);
        *p_lut++ = cv::saturate_cast<uchar>((*red_interpolator)[value]);
    }
}
