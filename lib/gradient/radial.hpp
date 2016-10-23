#ifndef __FILATTI_RADIAL_HPP__
#define __FILATTI_RADIAL_HPP__

#include "gradient.hpp"

namespace filatti {
    namespace gradient {
        template<typename T>
        class Radial : public Gradient {
        private:
            T _center_x;
            T _center_y;
            T _radius;
            T _feathering;
            bool _is_elliptic;
            bool _reserve;

            void create_circle(cv::Mat& dst, int center_x, int center_y, int radius, T feathering, bool reserve) const {
                T radius_pow2 = T(radius * radius);
                T feathering_width = T(feathering * radius);
                T inner_radius = T(radius - feathering_width);
                T inner_radius_pow2 = inner_radius * inner_radius;

                for (int row = 0, height = dst.rows; row < height; ++row) {
                    uchar* p = dst.ptr<uchar>(row);
                    for (int col = 0, width = dst.cols; col < width; ++col) {
                        int x = col - center_x;
                        int y = row - center_y;
                        int x_pow2 = x * x;
                        int y_pow2 = y * y;
                        T x_y_pow2 = x_pow2 + y_pow2;

                        T blend;

                        if (x_y_pow2 > radius_pow2) {
                            blend = T(0);
                        } else if (x_y_pow2 > inner_radius_pow2) {
                            blend = T(1) - (std::sqrt(x_y_pow2) - inner_radius) / feathering_width;
                        } else {
                            blend = T(1);
                        }

                        T value = T(255) * (reserve ? T(1) - blend : blend);
                        p[col] = cv::saturate_cast<uchar>(value);
                    }
                }
            }

            void create_ellipse(cv::Mat& dst,
                                int center_x, int center_y,
                                int radius_width, int radius_height,
                                T feathering, bool reserve) const {
                T radius_width_pow2 = T(radius_width * radius_width);
                T radius_height_pow2 = T(radius_height * radius_height);
                T feathering_radius = T(1) - feathering;

                for (int row = 0, height = dst.rows; row < height; ++row) {
                    uchar* p = dst.ptr<uchar>(row);
                    for (int col = 0, width = dst.cols; col < width; ++col) {
                        int x = col - center_x;
                        int y = row - center_y;
                        T x_pow2 = x * x;
                        T y_pow2 = y * y;

                        T blend;

                        T d = std::sqrt((x_pow2 / radius_width_pow2) + (y_pow2 / radius_height_pow2));
                        if (d >= T(1)) {
                            blend = T(0);
                        } else if (d <= feathering_radius) {
                            blend = T(1);
                        } else {
                            blend = T(1) - (d - feathering_radius) / feathering;
                        }

                        T value = T(255) * (reserve ? T(1) - blend : blend);
                        p[col] = cv::saturate_cast<uchar>(value);
                    }
                }
            }

        public:
            Radial(T center_x, T center_y, T radius, T feathering, bool is_elliptic, bool reserve = false)
                    : _center_x(center_x),
                      _center_y(center_y),
                      _radius(radius),
                      _feathering(feathering),
                      _is_elliptic(is_elliptic),
                      _reserve(reserve) {
            }

            virtual ~Radial() { }

            virtual bool create(cv::Mat& dst) const override {
                if (dst.empty() || dst.type() != CV_8UC1) {
                    return false;
                }

                int width = dst.cols;
                int height = dst.rows;

                int center_x = int(width * _center_x);
                int center_y = int(height * _center_y);

                int radius_width = int(width / 2 * _radius);
                int radius_height = int(height / 2 * _radius);

                if (!_is_elliptic || width == height) {
                    create_circle(dst, center_x, center_y, radius_width, _feathering, _reserve);
                } else {
                    create_ellipse(dst, center_x, center_y, radius_width, radius_height, _feathering, _reserve);
                }

                return true;
            }
        };
    }
}

#endif //__FILATTI_RADIAL_HPP__
