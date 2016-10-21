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

float Vibrance::get_vibrance() const noexcept {
    return _vibrance;
}

void Vibrance::set_vibrance(float vibrance) {
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
            float b = float(*p_src_col++);
            float g = float(*p_src_col++);
            float r = float(*p_src_col++);

            float average = (b + g + r) / 3.0f;
            float max = std::max(std::max(b, g), r);
            float adjust = (max - average) / 127.0f * _vibrance;

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
