#include <filatti/contrast.hpp>
#include <filatti/contract.hpp>

using namespace filatti;

Contrast::Contrast() : Dirty(true), _contrast(CONTRAST_NONE) {
}

Contrast::~Contrast() {
}

bool Contrast::has_effect() const noexcept {
    return _contrast != CONTRAST_NONE;
}

Contrast::Type Contrast::get_contrast() const noexcept {
    return _contrast;
}

void Contrast::set_contrast(Type contrast) {
    PRECONDITION(contrast >= CONTRAST_MIN && contrast <= CONTRAST_MAX, "Contrast is out of range");
    synchronize([=] {
        _contrast = contrast;
        make_dirty();
    });
}

bool Contrast::apply(const cv::Mat& src, cv::Mat& dst) {
    if (!has_effect()) {
        return false;
    } else {
        synchronize([this] {
            if (make_clean_if_dirty()) {
                build_lut();
            }
        });

        cv::Mat lut(_lut.size(), CV_8UC3);
        cv::mixChannels(_lut, lut, std::vector<int>{0, 0, 0, 1, 0, 2});
        cv::LUT(src, lut, dst);
        return true;
    }
}

void Contrast::build_lut() {
    if (_lut.empty()) {
        create_lut(_lut, CV_8UC1);
    }

    uchar* p_lut = _lut.ptr<uchar>(0);
    for (int i = 0, j = _lut.rows; i < j; ++i) {
        p_lut[i] = cv::saturate_cast<uchar>(Type(i - 127) * _contrast + Type(127));
    }
}
