#include <iostream>

#include <opencv2/opencv.hpp>
#include <filatti/grayscale.hpp>

#include "test.hpp"

#define WINDOW_NAME "Gray Adjustment"
#define MODE_TRACKBAR_NAME "Mode"
#define BLUR_KERNEL_SIZE_TRACKBAR_NAME "Blur kernel size"

cv::Mat src;
filatti::Grayscale grayscale;
int mode_trackbar_value = 0;
int blur_kernel_size = grayscale.get_blur_kernel_size();

void on_trackbar(int, void*) {
    grayscale.set_mode(filatti::Grayscale::Mode(mode_trackbar_value));
    grayscale.set_blur_kernel_size(blur_kernel_size);

    std::cout << "Mode=" << mode_trackbar_value
    << ", Blur kernel size=" << grayscale.get_blur_kernel_size()
    << std::endl;

    cv::Mat dst;
    auto before = get_current_milliseconds();
    if (!grayscale.apply(src, dst))
        dst = src;
    auto after = get_current_milliseconds();
    std::cout << "Spent: " << (after - before).count() << " ms" << std::endl;

    cv::imshow(WINDOW_NAME, dst);
}

int main() {
    src = cv::imread(IMAGE_FILE_2);

    cv::namedWindow(WINDOW_NAME, cv::WINDOW_NORMAL);
    cv::createTrackbar(MODE_TRACKBAR_NAME, WINDOW_NAME, &mode_trackbar_value, 3, on_trackbar);
    cv::createTrackbar(BLUR_KERNEL_SIZE_TRACKBAR_NAME, WINDOW_NAME, &blur_kernel_size, 30, on_trackbar);
    on_trackbar(0, nullptr);

    cv::waitKey(0);
    return 0;
}
