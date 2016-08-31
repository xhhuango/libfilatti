#include <iostream>

#include <opencv2/opencv.hpp>
#include "../lib/gradient/radial.hpp"

#include "test.hpp"

#define WINDOW_NAME "Radial Gradient Adjustment"

int main() {
    cv::Mat dst(450, 800, CV_8UC1);

    auto before = get_current_milliseconds();

    gradient::Radial radial(0.5, 0.5, 0.6, 0.8, true);
    radial.create(dst);

    auto after = get_current_milliseconds();
    std::cout << "Spent: " << (after - before).count() << " ms" << std::endl;

    cv::namedWindow(WINDOW_NAME, cv::WINDOW_NORMAL);
    cv::imshow(WINDOW_NAME, dst);

    cv::waitKey(0);
    return 0;
}