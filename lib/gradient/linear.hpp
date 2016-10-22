#ifndef __FILATTI_LINEAR_HPP__
#define __FILATTI_LINEAR_HPP__

#include "gradient.hpp"

#include <opencv2/opencv.hpp>

namespace filatti {
    namespace gradient {
        template<typename T>
        class Linear : public Gradient {
        private:
            T _center_x;
            T _center_y;
            T _radius;
            T _feathering;
            T _angle;
            bool _reserve;

            void build_gradient(cv::Mat& gradient, T feathering) const {
                int radius = gradient.rows / 2;
                T inner_radius = radius * feathering;

                cv::Mat col_mat(gradient.rows, 1, gradient.type());
                col_mat.setTo(255);
                uchar* p_col_mat = col_mat.ptr<uchar>(0);
                for (int row = 0, rows = col_mat.rows; row < inner_radius; ++row) {
                    uchar value = cv::saturate_cast<uchar>(T(255) * (T(row) / inner_radius));
                    p_col_mat[row] = value;
                    p_col_mat[rows - row] = value;
                }

                for (int col = 0, cols = gradient.cols; col < cols; ++col) {
                    col_mat.copyTo(gradient.col(col));
                }
            }

        public:
            Linear(T center_x, T center_y, T radius, T feathering, T angle, bool reserve = false)
                    : _center_x(center_x), _center_y(center_y),
                      _radius(radius), _feathering(feathering), _angle(angle),
                      _reserve(reserve) {
            }

            virtual ~Linear() { }

            virtual bool create(cv::Mat& dst) const override {
                if (dst.empty() || dst.type() != CV_8UC1) {
                    return false;
                }

                int width = (int) std::sqrt(std::pow(dst.rows, 2) + std::pow(dst.cols, 2)) * 2;
                cv::Mat gradient((int) (dst.rows * _radius), width, CV_8UC1);
                build_gradient(gradient, _feathering);

                cv::Point_<T> dst_center(T(dst.cols * _center_x), T(dst.rows * _center_y));
                cv::Point_<T> gradient_center(gradient.cols / 2, gradient.rows / 2);
                cv::Mat rotate = cv::getRotationMatrix2D(gradient_center, _angle, 1.0);
                rotate.at<T>(0, 2) += dst_center.x - gradient_center.x;
                rotate.at<T>(1, 2) += dst_center.y - gradient_center.y;

                cv::warpAffine(gradient, dst, rotate, dst.size());
                if (_reserve) {
                    dst = cv::Scalar_<uchar>(255) - dst;
                }

                return true;
            }
        };
    }
}

#endif //__FILATTI_LINEAR_HPP__
