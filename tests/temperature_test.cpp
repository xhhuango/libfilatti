#include <iostream>

#include <opencv2/opencv.hpp>
#include <filatti/temperature.hpp>

#include "test.hpp"

#define WINDOW_NAME "Temperature Adjustment"
#define KELVIN_TRACKBAR_NAME "Kelvin"
#define STRENGTH_TRACKBAR_NAME "Strength"

cv::Mat src;
filatti::Temperature temperature;
int kelvin_trackbar_value = (temperature.get_kelvin() - 1500) / 100;
int strength_trackbar_value = (int) (temperature.get_strength() * 200);

void on_trackbar(int, void*) {
    temperature.set_kelvin(kelvin_trackbar_value * 100 + 1500);
    temperature.set_strength(strength_trackbar_value / 200.0);

    std::cout << "kelvin=" << temperature.get_kelvin() << ", strength=" << temperature.get_strength() << std::endl;

    cv::Mat dst;
    auto before = get_current_milliseconds();
    if (!temperature.apply(src, dst))
        dst = src;
    auto after = get_current_milliseconds();

    std::cout << "Spent: " << (after - before).count() << " ms" << std::endl;

    cv::imshow(WINDOW_NAME, dst);
}

int main() {
    src = cv::imread(IMAGE_FILE_1);

    cv::namedWindow(WINDOW_NAME, cv::WINDOW_NORMAL);
    cv::createTrackbar(KELVIN_TRACKBAR_NAME, WINDOW_NAME, &kelvin_trackbar_value, 100, on_trackbar);
    cv::createTrackbar(STRENGTH_TRACKBAR_NAME, WINDOW_NAME, &strength_trackbar_value, 100, on_trackbar);
    on_trackbar(strength_trackbar_value, nullptr);

    cv::waitKey(0);
    return 0;
}
