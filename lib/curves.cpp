#include <filatti/curves.hpp>

#include "interpolator/spline.hpp"
#include "interpolator/linear.hpp"

using namespace filatti;

Curves::Curves() {
    _value_points = std::make_pair(POINTS_NONE, POINTS_NONE);
    _blue_points = std::make_pair(POINTS_NONE, POINTS_NONE);
    _green_points = std::make_pair(POINTS_NONE, POINTS_NONE);
    _red_points = std::make_pair(POINTS_NONE, POINTS_NONE);
}

Curves::~Curves() {
}

inline bool Curves::check_input_points(const std::vector<uchar>& from, const std::vector<uchar>& to) const {
    return !(!within((int) from.size(), POINTS_SIZE_MIN, POINTS_SIZE_MAX)
             || (from.size() != to.size())
             || from.front() != POINTS_NONE.front()
             || from.back() != POINTS_NONE.back());
}

std::vector<uchar> Curves::get_curves(const std::pair<std::vector<uchar>, std::vector<uchar>>& points) const {
    interpolator::Spline spline(std::vector<double>(points.first.begin(), points.first.end()),
                                std::vector<double>(points.second.begin(), points.second.end()));
    std::vector<uchar> curves(256);
    for (int i = 0; i < 256; ++i) {
        curves[i] = cv::saturate_cast<uchar>(spline[i]);
    }
    return curves;
}

bool Curves::set_value_points(std::vector<uchar> from, std::vector<uchar> to) {
    if (!check_input_points(from, to)) {
        return false;
    }

    _value_points = std::make_pair(std::move(from), std::move(to));
    release_lut();
    return true;
}

std::pair<std::vector<uchar>, std::vector<uchar>> Curves::get_value_points() const {
    return _value_points;
}

std::vector<uchar> Curves::get_value_curves() const {
    return get_curves(_value_points);
}

bool Curves::set_blue_points(std::vector<uchar> from, std::vector<uchar> to) {
    if (!check_input_points(from, to)) {
        return false;
    }

    _blue_points = std::make_pair(std::move(from), std::move(to));
    release_lut();
    return true;
}

std::pair<std::vector<uchar>, std::vector<uchar>> Curves::get_blue_points() const {
    return _blue_points;
}

std::vector<uchar> Curves::get_blue_curves() const {
    return get_curves(_blue_points);
}

bool Curves::set_green_points(std::vector<uchar> from, std::vector<uchar> to) {
    if (!check_input_points(from, to)) {
        return false;
    }

    _green_points = std::make_pair(std::move(from), std::move(to));
    release_lut();
    return true;
}

std::pair<std::vector<uchar>, std::vector<uchar>> Curves::get_green_points() const {
    return _green_points;
}

std::vector<uchar> Curves::get_green_curves() const {
    return get_curves(_green_points);
}

bool Curves::set_red_points(std::vector<uchar> from, std::vector<uchar> to) {
    if (!check_input_points(from, to)) {
        return false;
    }

    _red_points = std::make_pair(std::move(from), std::move(to));
    release_lut();
    return true;
}

std::pair<std::vector<uchar>, std::vector<uchar>> Curves::get_red_points() const {
    return _red_points;
}

std::vector<uchar> Curves::get_red_curves() const {
    return get_curves(_red_points);
}

bool Curves::has_effect() const {
    return !(_value_points.first == POINTS_NONE && _value_points.second == POINTS_NONE
             && _blue_points.first == POINTS_NONE && _blue_points.second == POINTS_NONE
             && _green_points.first == POINTS_NONE && _green_points.second == POINTS_NONE
             && _red_points.first == POINTS_NONE && _red_points.second == POINTS_NONE);
}

bool Curves::apply(const cv::Mat& src, cv::Mat& dst) {
    if (!has_effect()) {
        return false;
    } else {
        if (_lut.empty()) {
            build_lut();
        }
        cv::LUT(src, _lut, dst);
        return true;
    }
}

void Curves::release_lut() {
    if (!_lut.empty()) {
        _lut.release();
    }
}

void Curves::build_lut() {
    if (_lut.empty()) {
        _lut.create(256, 1, CV_8UC3);
    }

    interpolator::Interpolator* interpolator;

    if (_value_points.first.size() > 2) {
        interpolator =
                new interpolator::Spline(std::vector<double>(_value_points.first.begin(), _value_points.first.end()),
                                         std::vector<double>(_value_points.second.begin(), _value_points.second.end()));
    } else {
        interpolator =
                new interpolator::Linear(std::vector<double>(_value_points.first.begin(), _value_points.first.end()),
                                         std::vector<double>(_value_points.second.begin(), _value_points.second.end()));
    }
    std::unique_ptr<interpolator::Interpolator> value_interpolator(interpolator);

    if (_blue_points.first.size() > 2) {
        interpolator =
                new interpolator::Spline(std::vector<double>(_blue_points.first.begin(), _blue_points.first.end()),
                                         std::vector<double>(_blue_points.second.begin(), _blue_points.second.end()));
    } else {
        interpolator =
                new interpolator::Linear(std::vector<double>(_blue_points.first.begin(), _blue_points.first.end()),
                                         std::vector<double>(_blue_points.second.begin(), _blue_points.second.end()));
    }
    std::unique_ptr<interpolator::Interpolator> blue_interpolator(interpolator);

    if (_green_points.first.size() > 2) {
        interpolator =
                new interpolator::Spline(std::vector<double>(_green_points.first.begin(), _green_points.first.end()),
                                         std::vector<double>(_green_points.second.begin(), _green_points.second.end()));
    } else {
        interpolator =
                new interpolator::Linear(std::vector<double>(_green_points.first.begin(), _green_points.first.end()),
                                         std::vector<double>(_green_points.second.begin(), _green_points.second.end()));
    }
    std::unique_ptr<interpolator::Interpolator> green_interpolator(interpolator);

    if (_red_points.first.size() > 2) {
        interpolator =
                new interpolator::Spline(std::vector<double>(_red_points.first.begin(), _red_points.first.end()),
                                         std::vector<double>(_red_points.second.begin(), _red_points.second.end()));
    } else {
        interpolator =
                new interpolator::Linear(std::vector<double>(_red_points.first.begin(), _red_points.first.end()),
                                         std::vector<double>(_red_points.second.begin(), _red_points.second.end()));
    }
    std::unique_ptr<interpolator::Interpolator> red_interpolator(interpolator);

    for (int i = 0; i < 256; ++i) {
        double value = (*value_interpolator)[i];
        _lut.at<cv::Vec3b>(i, 0) = cv::Vec3b{cv::saturate_cast<uchar>((*blue_interpolator)[value]),
                                             cv::saturate_cast<uchar>((*green_interpolator)[value]),
                                             cv::saturate_cast<uchar>((*red_interpolator)[value])};
    }
}
