#include <iostream>

#include <opencv2/opencv.hpp>
#include <filatti/exposure.hpp>

#include "test.hpp"

#define WINDOW_NAME "Exposure Adjustment"
#define EXPOSURE_TRACKBAR_NAME "Exposure"

cv::Mat src;
filatti::Exposure exposure;
int exposure_trackbar_value = (int) (exposure.get_exposure() * 100.0f + 200);

void on_trackbar(int, void*) {
    exposure.set_exposure((exposure_trackbar_value - 200) / 100.0f);

    std::cout << "Exposure=" << exposure.get_exposure() << std::endl;

    cv::Mat dst;
    auto before = get_current_milliseconds();
    if (!exposure.apply(src, dst))
        dst = src;
    auto after = get_current_milliseconds();
    std::cout << "Spent: " << (after - before).count() << " ms" << std::endl;

    cv::imshow(WINDOW_NAME, dst);
}

int main() {
    src = cv::imread(IMAGE_FILE_2);

    cv::namedWindow(WINDOW_NAME, cv::WINDOW_NORMAL);
    cv::createTrackbar(EXPOSURE_TRACKBAR_NAME, WINDOW_NAME, &exposure_trackbar_value, 400, on_trackbar);
    on_trackbar(0, nullptr);

    cv::waitKey(0);
    return 0;
}
