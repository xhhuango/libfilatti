#ifndef __FILATTI_VIBRANCE_HPP__
#define __FILATTI_VIBRANCE_HPP__

#include <filatti/adjustment.hpp>
#include <filatti/sharpness.hpp>

namespace filatti {
    class Vibrance : public Adjustment, public Synchronous {
    private:
        double _vibrance;

        void blend(const cv::Mat& src, cv::Mat& dst) const;

    protected:
        virtual bool has_effect() const noexcept override;

    public:
        static constexpr double VIBRANCE_MAX = 3;
        static constexpr double VIBRANCE_MIN = -2;
        static constexpr double VIBRANCE_NONE = 0;

        Vibrance();

        ~Vibrance();

        double get_vibrance() const noexcept;

        void set_vibrance(double vibrance);

        virtual bool apply(const cv::Mat& src, cv::Mat& dst) override;
    };
}

#endif //__FILATTI_VIBRANCE_HPP__
