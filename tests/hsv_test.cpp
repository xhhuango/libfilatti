#include <iostream>

#include <opencv2/opencv.hpp>
#include <filatti/hsv.hpp>

#include "test.hpp"

#define WINDOW_NAME "HSV Adjustment"
#define L_TRACKBAR_NAME "Brightness"
#define S_TRACKBAR_NAME "Saturation"

cv::Mat src;
filatti::Hsv hsv;
int l_trackbar_value = (int) (hsv.get_brightness() * 100) + 100;
int s_trackbar_value = (int) (hsv.get_saturation() * 100) + 100;

void on_trackbar(int, void*) {
    hsv.set_brightness((l_trackbar_value - 100) / 100.0);
    hsv.set_saturation((s_trackbar_value - 100) / 100.0);

    cv::Mat dst;
    auto before = get_current_milliseconds();
    if (!hsv.apply(src, dst))
        dst = src;
    auto after = get_current_milliseconds();

    std::cout << "Spent: " << (after - before).count() << " ms" << std::endl;

    cv::imshow(WINDOW_NAME, dst);
}

int main() {
    src = cv::imread(IMAGE_FILE_1);

    cv::namedWindow(WINDOW_NAME, cv::WINDOW_NORMAL);
    cv::createTrackbar(L_TRACKBAR_NAME, WINDOW_NAME, &l_trackbar_value, 200, on_trackbar);
    cv::createTrackbar(S_TRACKBAR_NAME, WINDOW_NAME, &s_trackbar_value, 200, on_trackbar);
    on_trackbar(0, nullptr);

    cv::waitKey(0);
    return 0;
}
