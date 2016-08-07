#include <iostream>

#include <opencv2/opencv.hpp>
#include <filatti/curves.hpp>

#include "test.hpp"

#define WINDOW_NAME "Curves Adjustment"
#define SHADOWS_TRACKBAR_NAME "Shadows"
#define MIDTONES_TRACKBAR_NAME "Midtones"
#define HIGHLIGHTS_TRACKBAR_NAME "Highlights"

#define SHADOWS 64
#define MIDTONES 128
#define HIGHLIGHTS 192

cv::Mat src;
filatti::Curves curves;
int shadows_trackbar_value = SHADOWS;
int midtones_trackbar_value = MIDTONES;
int highlights_trackbar_value = HIGHLIGHTS;

void on_trackbar(int, void*) {
    curves.set_value_points(std::vector<uchar>{0, SHADOWS, MIDTONES, HIGHLIGHTS, 255},
                            std::vector<uchar>{0,
                                               (uchar) shadows_trackbar_value,
                                               (uchar) midtones_trackbar_value,
                                               (uchar) highlights_trackbar_value,
                                               255});

    std::cout << "Shadows=" << shadows_trackbar_value
    << ", Midtones=" << midtones_trackbar_value
    << ", Highlights=" << highlights_trackbar_value << std::endl;

    cv::Mat dst;
    auto before = get_current_milliseconds();
    if (!curves.apply(src, dst))
        dst = src;
    auto after = get_current_milliseconds();

    std::cout << "Spent: " << (after - before).count() << " ms" << std::endl;

    cv::imshow(WINDOW_NAME, dst);
}

int main() {
    src = cv::imread(IMAGE_FILE_2);

    cv::namedWindow(WINDOW_NAME, cv::WINDOW_NORMAL);
    cv::createTrackbar(SHADOWS_TRACKBAR_NAME, WINDOW_NAME, &shadows_trackbar_value, 255, on_trackbar);
    cv::createTrackbar(MIDTONES_TRACKBAR_NAME, WINDOW_NAME, &midtones_trackbar_value, 255, on_trackbar);
    cv::createTrackbar(HIGHLIGHTS_TRACKBAR_NAME, WINDOW_NAME, &highlights_trackbar_value, 255, on_trackbar);
    on_trackbar(0, nullptr);

    cv::waitKey(0);
    return 0;
}
