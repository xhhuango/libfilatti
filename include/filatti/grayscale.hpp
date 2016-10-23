#ifndef __FILATTI_GRAY_HPP__
#define __FILATTI_GRAY_HPP__

#include <filatti/adjustment.hpp>

namespace filatti {
    class Grayscale : public Adjustment {
    public:
        enum class Mode : unsigned int {
            NONE = 0,
            GRAYSCALE = 1,
            BLACK_WHITE = 2,
            PENCIL_SKETCH = 3,
        };

    private:
        Mode _mode;
        unsigned int _blur_kernel_size;

    protected:
        virtual bool has_effect() const noexcept override;

    public:
        static const unsigned int BLUR_KERNEL_SIZE_MIN = 1;
        static const unsigned int BLUR_KERNEL_SIZE_MAX = 30;

        Grayscale();

        virtual ~Grayscale();

        Mode get_mode() const noexcept;

        void set_mode(Mode mode) noexcept;

        unsigned int get_blur_kernel_size() const noexcept;

        void set_blur_kernel_size(unsigned int blur_kernel_size);

        virtual bool apply(const cv::Mat& src, cv::Mat& dst) override;
    };
}

#endif //__FILATTI_GRAY_HPP__
