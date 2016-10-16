#include <iostream>

#include <opencv2/opencv.hpp>
#include "../lib/gradient/linear.cpp"

#include "test.hpp"

#define WINDOW_NAME "Linear Gradient Adjustment"

int main() {
    cv::Mat dst(400, 400, CV_8UC1);

    auto before = get_current_milliseconds();

    filatti::gradient::Linear linear(0.1, 0.1, 0.5, .8, -60);
    linear.create(dst);

    auto after = get_current_milliseconds();
    std::cout << "Spent: " << (after - before).count() << " ms" << std::endl;

    cv::namedWindow(WINDOW_NAME, cv::WINDOW_NORMAL);
    cv::imshow(WINDOW_NAME, dst);

    cv::waitKey(0);
    return 0;
}