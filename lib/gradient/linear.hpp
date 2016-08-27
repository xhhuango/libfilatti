#ifndef __FILATTI_LINEAR_HPP__
#define __FILATTI_LINEAR_HPP__

#include "gradient.hpp"

namespace gradient {
    class Linear : public Gradient {
    public:
        Linear();

        virtual ~Linear() { }

        virtual bool create(cv::Mat& dst) const override;
    };
}

#endif //__FILATTI_LINEAR_HPP__
