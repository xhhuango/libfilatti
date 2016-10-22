#ifndef __FILATTI_VIBRANCE_HPP__
#define __FILATTI_VIBRANCE_HPP__

#include <filatti/adjustment.hpp>
#include <filatti/sharpness.hpp>

namespace filatti {
    class Vibrance : public Adjustment, public Synchronous {
    public:
        using Type = float;

    private:
        Type _vibrance;

        void blend(const cv::Mat& src, cv::Mat& dst) const;

    protected:
        virtual bool has_effect() const noexcept override;

    public:
        static constexpr Type VIBRANCE_MAX = Type(5);
        static constexpr Type VIBRANCE_MIN = Type(-2);
        static constexpr Type VIBRANCE_NONE = Type(0);

        Vibrance();

        ~Vibrance();

        Type get_vibrance() const noexcept;

        void set_vibrance(Type vibrance);

        virtual bool apply(const cv::Mat& src, cv::Mat& dst) override;
    };
}

#endif //__FILATTI_VIBRANCE_HPP__
