#include <iostream>

#include <opencv2/opencv.hpp>
#include <filatti/white_balance.hpp>

#include "test.hpp"

#define WINDOW_NAME "White Balance Adjustment"
#define TRACKBAR_NAME "Percent"

cv::Mat src;
filatti::WhiteBalance whiteBalance;
int trackbar_value = (int) (whiteBalance.get_percent() * 100);

void on_trackbar(int, void*) {
    whiteBalance.set_percent(trackbar_value / 100.0);

    std::cout << "Percent=" << whiteBalance.get_percent() << std::endl;

    cv::Mat dst;
    auto before = get_current_milliseconds();
    if (!whiteBalance.apply(src, dst))
        dst = src;
    auto after = get_current_milliseconds();
    std::cout << "Spent: " << (after - before).count() << " ms" << std::endl;

    cv::imshow(WINDOW_NAME, dst);
}

int main() {
    src = cv::imread(IMAGE_FILE_2);

    cv::namedWindow(WINDOW_NAME, cv::WINDOW_NORMAL);
    cv::createTrackbar(TRACKBAR_NAME, WINDOW_NAME, &trackbar_value, 10, on_trackbar);
    on_trackbar(trackbar_value, nullptr);

    cv::waitKey(0);
    return 0;
}
