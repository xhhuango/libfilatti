#include <filatti/exposure.hpp>
#include <filatti/contract.hpp>

using namespace filatti;

Exposure::Exposure() : Dirty(true), _exposure(EXPOSURE_NONE) {
}

Exposure::~Exposure() {
}

bool Exposure::has_effect() const noexcept {
    return _exposure != EXPOSURE_NONE;
}

Exposure::Type Exposure::get_exposure() const noexcept {
    return _exposure;
}

void Exposure::set_exposure(Type exposure) {
    PRECONDITION(exposure >= EXPOSURE_MIN && exposure <= EXPOSURE_MAX, "Exposure is out of range");
    synchronize([=] {
        _exposure = exposure;
        make_dirty();
    });
}

bool Exposure::apply(const cv::Mat& src, cv::Mat& dst) {
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

void Exposure::build_lut() {
    if (_lut.empty()) {
        create_lut(_lut, CV_8UC1);
    }

    uchar* p_lut = _lut.ptr<uchar>(0);
    for (int i = 0, j = _lut.rows; i < j; ++i) {
        Type value = Type(i) / Type(255);
        value *= std::pow(Type(2), _exposure);
        value *= Type(255);
        p_lut[i] = cv::saturate_cast<uchar>(value);
    }
}
