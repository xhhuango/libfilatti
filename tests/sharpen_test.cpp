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
    cv::Mat tmp(src.size(), src.type());

    sharpen.set_sharpen(trackbar_value / 100.0);
    if (sharpen.apply(src, tmp)) {
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
    cv::createTrackbar(TRACKBAR_NAME, WINDOW_NAME, &trackbar_value, 100, on_trackbar);
    on_trackbar(trackbar_value, nullptr);

    cv::waitKey(0);
    return 0;
}
