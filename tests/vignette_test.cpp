#include <iostream>

#include <opencv2/opencv.hpp>
#include <filatti/vignette.hpp>

#include "test.hpp"

#define WINDOW_NAME "Vignette Adjustment"
#define TRACKBAR_NAME "Vignette"

cv::Mat src;
filatti::Vignette vignette;
int trackbar_value = 100;

void on_trackbar(int, void*) {
//    vignette.set_brightness((trackbar_value - 100) / 100.0);

    cv::Mat tmp(src.size(), CV_64FC1);
    auto before = get_current_milliseconds();
    vignette.apply(src, tmp);
    auto after = get_current_milliseconds();

    std::cout << "Spent: " << (after - before).count() << " ms" << std::endl;

    cv::imshow(WINDOW_NAME, tmp);
}

int main() {
    src = cv::imread(IMAGE_FILE_1);

    cv::namedWindow(WINDOW_NAME, cv::WINDOW_NORMAL);
    cv::createTrackbar(TRACKBAR_NAME, WINDOW_NAME, &trackbar_value, 200, on_trackbar);
    on_trackbar(trackbar_value, nullptr);

    cv::waitKey(0);
    return 0;
}
