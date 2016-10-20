#include <iostream>

#include <opencv2/opencv.hpp>
#include <filatti/vibrance.hpp>

#include "test.hpp"

#define WINDOW_NAME "Vibrance Adjustment"
#define TRACKBAR_NAME "Vibrance"

cv::Mat src;
filatti::Vibrance vibrance;
int trackbar_value = (int) (vibrance.get_vibrance() * 100 + 200);

void on_trackbar(int, void*) {
    vibrance.set_vibrance((trackbar_value - 200) / 100.0);

    std::cout << "Vibrance=" << vibrance.get_vibrance() << std::endl;

    cv::Mat dst;
    auto before = get_current_milliseconds();
    if (!vibrance.apply(src, dst))
        dst = src;
    auto after = get_current_milliseconds();
    std::cout << "Spent: " << (after - before).count() << " ms" << std::endl;

    cv::imshow(WINDOW_NAME, dst);
}

int main() {
    src = cv::imread(IMAGE_FILE_1);

    cv::namedWindow(WINDOW_NAME, cv::WINDOW_NORMAL);
    cv::createTrackbar(TRACKBAR_NAME, WINDOW_NAME, &trackbar_value, 500, on_trackbar);
    on_trackbar(trackbar_value, nullptr);

    cv::waitKey(0);
    return 0;
}
