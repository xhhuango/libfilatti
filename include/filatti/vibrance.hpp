#ifndef __FILATTI_VIBRANCE_HPP__
#define __FILATTI_VIBRANCE_HPP__

#include <filatti/adjustment.hpp>
#include <filatti/sharpness.hpp>

namespace filatti {
    class Vibrance : public Adjustment, public Synchronous {
    private:
        float _vibrance;

        void blend(const cv::Mat& src, cv::Mat& dst) const;

    protected:
        virtual bool has_effect() const noexcept override;

    public:
        static constexpr float VIBRANCE_MAX = 3;
        static constexpr float VIBRANCE_MIN = -2;
        static constexpr float VIBRANCE_NONE = 0;

        Vibrance();

        ~Vibrance();

        float get_vibrance() const noexcept;

        void set_vibrance(float vibrance);

        virtual bool apply(const cv::Mat& src, cv::Mat& dst) override;
    };
}

#endif //__FILATTI_VIBRANCE_HPP__
