#include <iostream>

#include <opencv2/opencv.hpp>
#include <filatti/temperature.hpp>

#include "test.hpp"

#define WINDOW_NAME "Temperature Adjustment"
#define TEMPERATURE_TRACKBAR_NAME "Temperature"

cv::Mat src;
filatti::Temperature temperature;
int temperature_trackbar_value = (int) (temperature.get_temperature() * 100. + 50);

void on_trackbar(int, void*) {
    temperature.set_temperature((temperature_trackbar_value - 50) / 100.);

    std::cout << "temperature=" << temperature.get_temperature() << std::endl;

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
    cv::createTrackbar(TEMPERATURE_TRACKBAR_NAME, WINDOW_NAME, &temperature_trackbar_value, 100, on_trackbar);
    on_trackbar(0, nullptr);

    cv::waitKey(0);
    return 0;
}
