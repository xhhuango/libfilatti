#ifndef __FILATTI_ADJUSTMENT_HPP__
#define __FILATTI_ADJUSTMENT_HPP__

#include <opencv2/core.hpp>

namespace filatti {
    using ToneRawType = unsigned int;
    enum class Tone : ToneRawType {
        SHADOWS = 0,
        MIDTONES = 1,
        HIGHLIGHTS = 2,
        TOTAL = 3
    };

    inline void create_lut(cv::Mat& lut, int type) {
        lut.create(256, 1, type);
    }

    class Adjustment {
    public:
        virtual ~Adjustment() { }

        virtual bool has_effect() const noexcept = 0;

        virtual bool apply(const cv::Mat& src, cv::Mat& dst) = 0;
    };
}

#endif //__FILATTI_ADJUSTMENT_HPP__
