#include <iostream>

#include <opencv2/opencv.hpp>
#include <filatti/vignette.hpp>

#include "test.hpp"

#define WINDOW_NAME "Vignette Adjustment"
#define RADIUS_TRACKBAR_NAME "Radius"
#define STRENGTH_TRACKBAR_NAME "Strength"
#define FEATHERING_TRACKBAR_NAME "Feathering"

cv::Mat src;
filatti::Vignette vignette;
int radius_trackbar_value = 65;
int feathering_trackbar_value = 35;
int strength_trackbar_value = 75;

void on_trackbar(int, void*) {
    vignette.set_radius(radius_trackbar_value / 100.0);
    vignette.set_feathering(feathering_trackbar_value / 100.0);
    vignette.set_strength(strength_trackbar_value / 100.0);

    std::cout << "Radius=" << radius_trackbar_value
    << ", Feathering=" << feathering_trackbar_value
    << ", Strength=" << strength_trackbar_value << std::endl;

    cv::Mat dst;
    auto before = get_current_milliseconds();
    vignette.apply(src, dst);
    auto after = get_current_milliseconds();

    std::cout << "Spent: " << (after - before).count() << " ms" << std::endl;

    cv::imshow(WINDOW_NAME, dst);
}

int main() {
    src = cv::imread(IMAGE_FILE_1);

    cv::namedWindow(WINDOW_NAME, cv::WINDOW_NORMAL);
    cv::createTrackbar(RADIUS_TRACKBAR_NAME, WINDOW_NAME, &radius_trackbar_value, 100, on_trackbar);
    cv::createTrackbar(FEATHERING_TRACKBAR_NAME, WINDOW_NAME, &feathering_trackbar_value, 100, on_trackbar);
    cv::createTrackbar(STRENGTH_TRACKBAR_NAME, WINDOW_NAME, &strength_trackbar_value, 100, on_trackbar);
    on_trackbar(0, nullptr);

    cv::waitKey(0);
    return 0;
}
