#include <iostream>

#include <opencv2/opencv.hpp>
#include <filatti/sharpen.hpp>

#include "test.hpp"

#define WINDOW_NAME "Sharpen Adjustment"
#define TRACKBAR_NAME "Sharpen"

cv::Mat src;
filatti::Sharpen sharpen;
int trackbar_value = 0;

void on_trackbar(int, void*) {
    sharpen.set_sharpen(trackbar_value / 100.0);

    cv::Mat tmp(src.size(), src.type());
    if (!sharpen.apply(src, tmp))
        tmp = src;
    cv::imshow(WINDOW_NAME, tmp);
}

int main() {
    src = cv::imread(IMAGE_FILE_1);

    cv::namedWindow(WINDOW_NAME, cv::WINDOW_NORMAL);
    cv::createTrackbar(TRACKBAR_NAME, WINDOW_NAME, &trackbar_value, 100, on_trackbar);
    on_trackbar(trackbar_value, nullptr);

    cv::waitKey(0);
    return 0;
}
