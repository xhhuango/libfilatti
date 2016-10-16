#include <iostream>

#include <opencv2/opencv.hpp>
#include <filatti/hls.hpp>

#include "test.hpp"

#define WINDOW_NAME "HSV Adjustment"
#define H_TRACKBAR_NAME "Hue"
#define L_TRACKBAR_NAME "Lightness"
#define S_TRACKBAR_NAME "Saturation"

cv::Mat src;
filatti::Hls hls;
int h_trackbar_value = hls.get_hue() + 180;
int l_trackbar_value = (int) (hls.get_lightness() * 100) + 100;
int s_trackbar_value = (int) (hls.get_saturation() * 100) + 100;

void on_trackbar(int, void*) {
    hls.set_hue(h_trackbar_value - 180);
    hls.set_lightness((l_trackbar_value - 100) / 100.0);
    hls.set_saturation((s_trackbar_value - 100) / 100.0);

    std::cout << "HUE=" << hls.get_hue()
    << ", Lightness=" << hls.get_lightness()
    << ", Saturation=" << hls.get_saturation()
    << std::endl;

    cv::Mat dst;
    auto before = get_current_milliseconds();
    if (!hls.apply(src, dst))
        dst = src;
    auto after = get_current_milliseconds();

    std::cout << "Spent: " << (after - before).count() << " ms" << std::endl;

    cv::imshow(WINDOW_NAME, dst);
}

int main() {
    src = cv::imread(IMAGE_FILE_1);

    cv::namedWindow(WINDOW_NAME, cv::WINDOW_NORMAL);
    cv::createTrackbar(H_TRACKBAR_NAME, WINDOW_NAME, &h_trackbar_value, 360, on_trackbar);
    cv::createTrackbar(L_TRACKBAR_NAME, WINDOW_NAME, &l_trackbar_value, 200, on_trackbar);
    cv::createTrackbar(S_TRACKBAR_NAME, WINDOW_NAME, &s_trackbar_value, 200, on_trackbar);
    on_trackbar(0, nullptr);

    cv::waitKey(0);
    return 0;
}
