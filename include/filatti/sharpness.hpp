#ifndef __FILATTI_SHARPNESS_HPP__
#define __FILATTI_SHARPNESS_HPP__

#include <filatti/adjustment.hpp>
#include <filatti/rebuild.hpp>
#include <filatti/synchronous.hpp>

namespace filatti {
    class Sharpness : public Adjustment, public Rebuild, public Synchronous {
    private:
        double _sharpness;
        cv::Mat _blurred;

        void build_blurred(const cv::Mat& src);

    protected:
        virtual bool has_effect() const noexcept override;

    public:
        static constexpr double SHARPNESS_MIN = 0;
        static constexpr double SHARPNESS_MAX = 20;
        static constexpr double SHARPNESS_NONE = SHARPNESS_MIN;

        Sharpness();

        virtual ~Sharpness();

        double get_sharpness() const noexcept;

        void set_sharpness(double sharpness);

        void set_rebuild_blurred(bool does_rebuild_blurred) noexcept;

        virtual bool apply(const cv::Mat& src, cv::Mat& dst) override;
    };
}

#endif //__FILATTI_SHARPNESS_HPP__
