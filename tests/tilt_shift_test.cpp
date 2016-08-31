#include <iostream>

#include <opencv2/opencv.hpp>
#include <filatti/tilt_shift.hpp>

#include "test.hpp"

#define WINDOW_NAME "Tilt Shift Adjustment"

#define MASK_TYPE_TRACKBAR_NAME "Mask Type"
#define RADIUS_TRACKBAR_NAME "Radius"
#define FEATHERING_TRACKBAR_NAME "Feathering"
#define ANGLE_TRACKBAR_NAME "Angle"

cv::Mat src;
filatti::TiltShift tiltShift;
int mask_type_value = 0;
int radius_trackbar_value = (int) (tiltShift.get_radius() * 100);
int feathering_trackbar_value = (int) (tiltShift.get_feathering() * 100);
int angle_trackbar_value = (int) tiltShift.get_angle();

void on_trackbar(int, void*) {
    tiltShift.set_mask_type(filatti::TiltShift::MaskType(mask_type_value));
    tiltShift.set_radius(radius_trackbar_value / 100.0);
    tiltShift.set_feathering(feathering_trackbar_value / 100.0);
    tiltShift.set_angle((double) angle_trackbar_value);

    std::cout << "Radius=" << radius_trackbar_value
    << ", Feathering=" << feathering_trackbar_value
    << ", Angle=" << angle_trackbar_value
    << std::endl;

    cv::Mat dst;
    auto before = get_current_milliseconds();
    if (!tiltShift.apply(src, dst))
        dst = src;
    auto after = get_current_milliseconds();

    std::cout << "Spent: " << (after - before).count() << " ms" << std::endl;

    cv::imshow(WINDOW_NAME, dst);
}

int main() {
    src = cv::imread(IMAGE_FILE_1);

    cv::namedWindow(WINDOW_NAME, cv::WINDOW_NORMAL);
    cv::createTrackbar(MASK_TYPE_TRACKBAR_NAME, WINDOW_NAME, &mask_type_value, 2, on_trackbar);
    cv::createTrackbar(RADIUS_TRACKBAR_NAME, WINDOW_NAME, &radius_trackbar_value, 200, on_trackbar);
    cv::createTrackbar(FEATHERING_TRACKBAR_NAME, WINDOW_NAME, &feathering_trackbar_value, 100, on_trackbar);
    cv::createTrackbar(ANGLE_TRACKBAR_NAME, WINDOW_NAME, &angle_trackbar_value, 360, on_trackbar);
    on_trackbar(0, nullptr);

    cv::waitKey(0);
    return 0;
}
