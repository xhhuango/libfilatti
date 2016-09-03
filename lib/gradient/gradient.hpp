#ifndef __FILATTI_GRADIENT_HPP__
#define __FILATTI_GRADIENT_HPP__

#include <opencv2/core.hpp>

namespace filatti {
    namespace gradient {
        class Gradient {
        public:
            virtual ~Gradient() { }

            virtual bool create(cv::Mat& dst) const = 0;
        };
    }
}

#endif //__FILATTI_GRADIENT_HPP__
