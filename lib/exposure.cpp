#include <filatti/exposure.hpp>
#include <filatti/contract.hpp>

using namespace filatti;

Exposure::Exposure() : Dirty(true), _exposure(EXPOSURE_NONE), _offset(OFFSET_NONE), _gamma(GAMMA_NONE) {
}

Exposure::~Exposure() {
}

bool Exposure::has_effect() const noexcept {
    return _exposure != EXPOSURE_NONE || _offset != OFFSET_NONE || _gamma != GAMMA_NONE;
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

Exposure::Type Exposure::get_offset() const noexcept {
    return _offset;
}

void Exposure::set_offset(Type offset) {
    PRECONDITION(offset >= OFFSET_MIN && offset <= OFFSET_MAX, "Offset is out of range");
    synchronize([=] {
        _offset = offset;
        make_dirty();
    });
}

Exposure::Type Exposure::get_gamma() const noexcept {
    return _gamma;
}

void Exposure::set_gamma(Type gamma) {
    PRECONDITION(gamma >= GAMMA_MIN && gamma <= GAMMA_MAX, "Gamma is out of range");
    synchronize([=] {
        _gamma = gamma;
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
        value += _offset;
        if (value > Type(0)) {
            value = std::pow(value, (Type(1) / _gamma));
        }
        value *= Type(255);
        p_lut[i] = cv::saturate_cast<uchar>(value);
    }
}
