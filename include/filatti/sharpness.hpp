#ifndef __FILATTI_SHARPNESS_HPP__
#define __FILATTI_SHARPNESS_HPP__

#include <filatti/adjustment.hpp>
#include <filatti/rebuild.hpp>
#include <filatti/synchronous.hpp>

namespace filatti {
    class Sharpness : public Adjustment, public Rebuild, public Synchronous {
    public:
        using Type = float;

    private:
        Type _sharpness;
        cv::Mat _blurred;

        void build_blurred(const cv::Mat& src);

    protected:
        virtual bool has_effect() const noexcept override;

    public:
        static constexpr Type SHARPNESS_MIN = Type(0);
        static constexpr Type SHARPNESS_MAX = Type(20);
        static constexpr Type SHARPNESS_NONE = SHARPNESS_MIN;

        Sharpness();

        virtual ~Sharpness();

        Type get_sharpness() const noexcept;

        void set_sharpness(Type sharpness);

        void set_rebuild_blurred(bool does_rebuild_blurred) noexcept;

        virtual bool apply(const cv::Mat& src, cv::Mat& dst) override;
    };
}

#endif //__FILATTI_SHARPNESS_HPP__
