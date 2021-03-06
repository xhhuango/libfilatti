#include <iostream>

#include <opencv2/opencv.hpp>
#include <filatti/sharpness.hpp>

#include "test.hpp"

#define WINDOW_NAME "Sharpness Adjustment"
#define TRACKBAR_NAME "Sharpness"

cv::Mat src;
filatti::Sharpness sharpness;
int trackbar_value = (int) (sharpness.get_sharpness() * 100);

void on_trackbar(int, void*) {
    sharpness.set_sharpness(trackbar_value / 100.0);

    std::cout << "Sharpness=" << trackbar_value << std::endl;

    cv::Mat dst;
    auto before = get_current_milliseconds();
    if (!sharpness.apply(src, dst))
        dst = src;
    auto after = get_current_milliseconds();
    std::cout << "Spent: " << (after - before).count() << " ms" << std::endl;

    cv::imshow(WINDOW_NAME, dst);
}

int main() {
    src = cv::imread(IMAGE_FILE_1);

    cv::namedWindow(WINDOW_NAME, cv::WINDOW_NORMAL);
    cv::createTrackbar(TRACKBAR_NAME, WINDOW_NAME, &trackbar_value, 2000, on_trackbar);
    on_trackbar(0, nullptr);

    cv::waitKey(0);
    return 0;
}
