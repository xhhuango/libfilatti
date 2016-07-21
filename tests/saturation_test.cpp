#include <iostream>

#include <opencv2/opencv.hpp>
#include <filatti/saturation.hpp>

#include "test.hpp"

#define WINDOW_NAME "Saturation Adjustment"
#define TRACKBAR_NAME "Saturation"

cv::Mat src;
filatti::Saturation brightness;
int trackbar_value = 100;

void on_trackbar(int, void*) {
    brightness.set_saturation((trackbar_value - 100) / 100.0);

    cv::Mat tmp(src.size(), src.type());
    if (!brightness.apply(src, tmp))
        tmp = src;
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
