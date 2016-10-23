#include <filatti/grayscale.hpp>

#include <opencv2/opencv.hpp>
#include <filatti/contract.hpp>

using namespace filatti;

Grayscale::Grayscale() : _mode(Mode::NONE), _blur_kernel_size(BLUR_KERNEL_SIZE_MIN) {
}

Grayscale::~Grayscale() {
}

bool Grayscale::has_effect() const noexcept {
    return _mode != Mode::NONE;
}

Grayscale::Mode Grayscale::get_mode() const noexcept {
    return _mode;
}

void Grayscale::set_mode(Mode mode) noexcept {
    _mode = mode;
}

unsigned int Grayscale::get_blur_kernel_size() const noexcept {
    return _blur_kernel_size;
}

void Grayscale::set_blur_kernel_size(unsigned int blur_kernel_size) {
    PRECONDITION(blur_kernel_size >= BLUR_KERNEL_SIZE_MIN && blur_kernel_size <= BLUR_KERNEL_SIZE_MAX,
                 "Blur kernel size is out of range");
    _blur_kernel_size = blur_kernel_size;
}

bool Grayscale::apply(const cv::Mat& src, cv::Mat& dst) {
    if (!has_effect()) {
        return false;
    } else {
        cv::Mat gray;
        cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);

        switch (_mode) {
            case Mode::BLACK_WHITE:
                gray = gray > uchar(128);
                break;

            case Mode::PENCIL_SKETCH: {
                cv::Mat gray_blurred;
                cv::GaussianBlur(gray, gray_blurred, cv::Size(0, 0), _blur_kernel_size);
                cv::divide(gray, gray_blurred, gray, 256);
            }
                break;

            case Mode::GRAYSCALE:
            default:
                break;
        }

        cv::cvtColor(gray, dst, cv::COLOR_GRAY2BGR);
        return true;
    }
}
