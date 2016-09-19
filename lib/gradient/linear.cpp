#include <opencv2/opencv.hpp>
#include "linear.hpp"

using namespace filatti::gradient;

Linear::Linear(double center_x, double center_y, double radius, double feathering, double angle, bool reserve)
        : _center_x(center_x),
          _center_y(center_y),
          _radius(radius),
          _feathering(feathering),
          _angle(angle),
          _reserve(reserve) {
}

Linear::~Linear() {
}

void Linear::build_gradient(cv::Mat& gradient, double feathering) const {
    int radius = gradient.rows / 2;
    double inner_radius = radius * feathering;

    cv::Mat first_col = gradient.col(0);
    first_col.setTo(255);
    for (int row = 0, rows = first_col.rows; row < inner_radius; ++row) {
        uchar value = cv::saturate_cast<uchar>(255.0 * (row / inner_radius));
        first_col.at<uchar>(row) = value;
        first_col.at<uchar>(rows - row) = value;
    }

    for (int col = 1, cols = gradient.cols; col < cols; ++col) {
        first_col.copyTo(gradient.col(col));
    }
}

bool Linear::create(cv::Mat& dst) const {
    if (dst.empty() || dst.type() != CV_8UC1) {
        return false;
    }

    int width = (int) std::sqrt(std::pow(dst.rows, 2) + std::pow(dst.cols, 2)) * 2;
    cv::Mat gradient((int) (dst.rows * _radius), width, CV_8UC1);
    build_gradient(gradient, _feathering);

    cv::Point2f dst_center((float) (dst.cols * _center_x), (float) (dst.rows * _center_y));
    cv::Point2f gradient_center(gradient.cols / 2, gradient.rows / 2);
    cv::Mat rotate = cv::getRotationMatrix2D(gradient_center, _angle, 1.0);
    rotate.at<double>(0, 2) += dst_center.x - gradient_center.x;
    rotate.at<double>(1, 2) += dst_center.y - gradient_center.y;

    cv::warpAffine(gradient, dst, rotate, dst.size());
    if (_reserve) {
        dst = cv::Scalar_<uchar>(255) - dst;
    }

    return true;
}
