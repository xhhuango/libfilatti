#include <iostream>

#include <opencv2/opencv.hpp>
#include <filatti/saturation.hpp>

#include "test.hpp"

#define WINDOW_NAME "Saturation Adjustment"
#define TRACKBAR_NAME "Saturation"

cv::Mat src;
filatti::Saturation saturation;
int trackbar_value = 100;

void on_trackbar(int, void*) {
    cv::Mat tmp(src.size(), src.type());

    saturation.set_saturation((trackbar_value - 100) / 100.0);
    if (saturation.apply(src, tmp)) {
        cv::cvtColor(tmp, tmp, cv::COLOR_RGB2BGR);
    } else {
        cv::cvtColor(src, tmp, cv::COLOR_RGB2BGR);
    }
    cv::imshow(WINDOW_NAME, tmp);

}

int main() {
    src = cv::imread(IMAGE_FILE_1);
    cv::cvtColor(src, src, cv::COLOR_BGR2RGB);

    cv::namedWindow(WINDOW_NAME, cv::WINDOW_NORMAL);
    cv::createTrackbar(TRACKBAR_NAME, WINDOW_NAME, &trackbar_value, 200, on_trackbar);
    on_trackbar(trackbar_value, nullptr);

    cv::waitKey(0);
    return 0;
}
