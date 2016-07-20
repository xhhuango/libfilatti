#ifndef __FILATTI_SHARPEN_HPP__
#define __FILATTI_SHARPEN_HPP__

#include <opencv2/core.hpp>

namespace filatti {
    class Sharpen {
    private:
        double _sharpen;
        cv::Mat _blurred;

        void build_blurred(cv::Mat& src);

    public:
        const double SHARPEN_NONE = 0;
        const double SHARPEN_MIN = SHARPEN_NONE;
        const double SHARPEN_MAX = 1;

        Sharpen();
        ~Sharpen();
        double get_sharpen();
        bool set_sharpen(double sharpen);
        bool apply(cv::Mat& src, cv::Mat& dst);
    };
}

#endif //__FILATTI_SHARPEN_HPP__
