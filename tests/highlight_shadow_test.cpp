#include <iostream>

#include <opencv2/opencv.hpp>
#include <filatti/highlight_shadow.hpp>

#include "test.hpp"

#define WINDOW_NAME "Highlight/Shadow Adjustment"
#define SHADOW_AMOUNT_TRACKBAR_NAME "Shadow Amount"
#define SHADOW_TONE_WIDTH_TRACKBAR_NAME "Shadow Tone Width"
#define HIGHLIGHT_AMOUNT_TRACKBAR_NAME "Highlight Amount"
#define HIGHLIGHT_TONE_WIDTH_TRACKBAR_NAME "Highlight Tone Width"

cv::Mat src;
filatti::HighlightShadow highlightShadow;
int shadow_amount_trackbar_value = (int) (highlightShadow.get_amount(filatti::Tone::SHADOWS) * 100 + 100);
int shadow_tone_width_trackbar_value = (int) (highlightShadow.get_tone_width(filatti::Tone::SHADOWS) * 100);
int highlight_amount_trackbar_value = (int) (highlightShadow.get_amount(filatti::Tone::HIGHLIGHTS) * 100 + 100);
int highlight_tone_width_trackbar_value = (int) (highlightShadow.get_tone_width(filatti::Tone::HIGHLIGHTS) * 100);

void on_trackbar(int, void*) {
    highlightShadow.set_amount(filatti::Tone::SHADOWS, (shadow_amount_trackbar_value - 100) / 100.0);
    highlightShadow.set_tone_width(filatti::Tone::SHADOWS, shadow_tone_width_trackbar_value / 100.0);
    highlightShadow.set_amount(filatti::Tone::HIGHLIGHTS, (highlight_amount_trackbar_value - 100) / 100.0);
    highlightShadow.set_tone_width(filatti::Tone::HIGHLIGHTS, highlight_tone_width_trackbar_value / 100.0);

    std::cout << "Shadow: amount=" << highlightShadow.get_amount(filatti::Tone::SHADOWS)
            << ", tone width=" << highlightShadow.get_tone_width(filatti::Tone::SHADOWS)
            << "; Highlight: amount=" << highlightShadow.get_amount(filatti::Tone::HIGHLIGHTS)
            << ", tone width=" << highlightShadow.get_tone_width(filatti::Tone::HIGHLIGHTS)
            << std::endl;

    cv::Mat dst;
    auto before = get_current_milliseconds();
    if (!highlightShadow.apply(src, dst))
        dst = src;
    auto after = get_current_milliseconds();
    std::cout << "Spent: " << (after - before).count() << " ms" << std::endl;

    cv::imshow(WINDOW_NAME, dst);
}

int main() {
    src = cv::imread(IMAGE_FILE_2);

    cv::namedWindow(WINDOW_NAME, cv::WINDOW_NORMAL);

    cv::createTrackbar(SHADOW_AMOUNT_TRACKBAR_NAME, WINDOW_NAME, &shadow_amount_trackbar_value, 200, on_trackbar);
    cv::createTrackbar(SHADOW_TONE_WIDTH_TRACKBAR_NAME, WINDOW_NAME, &shadow_tone_width_trackbar_value, 50, on_trackbar);

    cv::createTrackbar(HIGHLIGHT_AMOUNT_TRACKBAR_NAME, WINDOW_NAME, &highlight_amount_trackbar_value, 200, on_trackbar);
    cv::createTrackbar(HIGHLIGHT_TONE_WIDTH_TRACKBAR_NAME, WINDOW_NAME, &highlight_tone_width_trackbar_value, 50, on_trackbar);

    on_trackbar(0, nullptr);

    cv::waitKey(0);
    return 0;
}

