#include <filatti/vibrance.hpp>
#include <filatti/contract.hpp>

using namespace filatti;

Vibrance::Vibrance() : _vibrance(VIBRANCE_NONE) {
}

Vibrance::~Vibrance() {
}

bool Vibrance::has_effect() const noexcept {
    return _vibrance != VIBRANCE_NONE;
}

Vibrance::Type Vibrance::get_vibrance() const noexcept {
    return _vibrance;
}

void Vibrance::set_vibrance(Type vibrance) {
    PRECONDITION(vibrance >= VIBRANCE_MIN && vibrance <= VIBRANCE_MAX, "Vibrance is out of range");
    synchronize([=] {
        _vibrance = vibrance;
    });
}

bool Vibrance::apply(const cv::Mat& src, cv::Mat& dst) {
    if (!has_effect()) {
        return false;
    } else {
        if (dst.empty()) {
            dst.create(src.size(), src.type());
        }
        synchronize([&] {
            blend(src, dst);
        });
        return true;
    }
}

void Vibrance::blend(const cv::Mat& src, cv::Mat& dst) const {
    long col_end_offset = src.cols * src.channels();

    for (int row = 0, rows = src.rows; row < rows; ++row) {
        const uchar* p_src_col = src.ptr<uchar>(row);
        const uchar* p_src_col_end = p_src_col + col_end_offset;
        uchar* p_dst = dst.ptr<uchar>(row);

        while (p_src_col < p_src_col_end) {
            Type b = Type(*p_src_col++);
            Type g = Type(*p_src_col++);
            Type r = Type(*p_src_col++);

            Type average = (b + g + r) / Type(3);
            Type max = std::max(std::max(b, g), r);
            Type adjust = (max - average) / Type(127) * _vibrance;

            if (b != max) {
                b -= (max - b) * adjust;
            }
            if (g != max) {
                g -= (max - g) * adjust;
            }
            if (r != max) {
                r -= (max - r) * adjust;
            }

            *p_dst++ = cv::saturate_cast<uchar>(b);
            *p_dst++ = cv::saturate_cast<uchar>(g);
            *p_dst++ = cv::saturate_cast<uchar>(r);
        }
    }
}
