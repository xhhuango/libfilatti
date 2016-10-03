#include <iostream>

#include <opencv2/opencv.hpp>
#include <filatti/color_balance.hpp>

#include "test.hpp"

#define WINDOW_NAME "Color Balance Adjustment"
#define RED_TRACKBAR_NAME "Red/Cyan"
#define GREEN_TRACKBAR_NAME "Green/Magenta"
#define BLUE_TRACKBAR_NAME "Blue/Yellow"

cv::Mat src;
filatti::ColorBalance color_balance;
filatti::Tone tone = filatti::Tone::MIDTONES;
int red_trackbar_value = color_balance.get_red_cyan(tone) + 100;
int green_trackbar_value = color_balance.get_green_magenta(tone) + 100;
int blue_trackbar_value = color_balance.get_blue_yellow(tone) + 100;

void on_trackbar(int, void*) {
    color_balance.set_red_cyan(tone, red_trackbar_value - 100);
    color_balance.set_green_magenta(tone, green_trackbar_value - 100);
    color_balance.set_blue_yellow(tone, blue_trackbar_value - 100);

    std::cout << "Cyan/Red=" << color_balance.get_red_cyan(tone)
    << ", Magenta/Green=" << color_balance.get_green_magenta(tone)
    << ", Yellow/Blue=" << color_balance.get_blue_yellow(tone) << std::endl;

    cv::Mat dst;
    auto before = get_current_milliseconds();
    if (!color_balance.apply(src, dst))
        dst = src;
    auto after = get_current_milliseconds();
    std::cout << "Spent: " << (after - before).count() << " ms" << std::endl;

    cv::imshow(WINDOW_NAME, dst);
}

int main() {
    src = cv::imread(IMAGE_FILE_2);

    cv::namedWindow(WINDOW_NAME, cv::WINDOW_NORMAL);
    cv::createTrackbar(RED_TRACKBAR_NAME, WINDOW_NAME, &red_trackbar_value, 200, on_trackbar);
    cv::createTrackbar(GREEN_TRACKBAR_NAME, WINDOW_NAME, &green_trackbar_value, 200, on_trackbar);
    cv::createTrackbar(BLUE_TRACKBAR_NAME, WINDOW_NAME, &blue_trackbar_value, 200, on_trackbar);
    on_trackbar(0, nullptr);

    cv::waitKey(0);
    return 0;
}
