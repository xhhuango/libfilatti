#ifndef __FILATTI_TILT_SHIFT_HPP__
#define __FILATTI_TILT_SHIFT_HPP__

#include <filatti/adjustment.hpp>
#include <filatti/dirty.hpp>
#include <filatti/rebuild.hpp>
#include <filatti/synchronous.hpp>

namespace filatti {
    class TiltShift : public Adjustment, public Dirty, public Rebuild, public Synchronous {
    public:
        enum class MaskType : unsigned int {
            CIRCULAR = 0,
            ELLIPTIC = 1,
            LINEAR = 2,
        };

        using Type = float;

    private:
        cv::Point_<Type> _center;
        Type _radius;
        Type _strength;
        Type _feathering;
        Type _angle;
        MaskType _mask_type;

        cv::Mat _mask;
        cv::Mat _blurred;

        void create_mask(const cv::Mat& src);

        void blur(const cv::Mat& src);

        void blend(const cv::Mat& src, cv::Mat& dst) const;

    protected:
        virtual bool has_effect() const noexcept override;

    public:
        static constexpr Type CENTER_MIN = Type(0);
        static constexpr Type CENTER_MAX = Type(1);

        static constexpr Type RADIUS_MIN = Type(0);
        static constexpr Type RADIUS_MAX = Type(2);
        static constexpr Type RADIUS_NONE = RADIUS_MAX;

        static constexpr Type STRENGTH_MIN = Type(0);
        static constexpr Type STRENGTH_MAX = Type(1);
        static constexpr Type STRENGTH_NONE = STRENGTH_MIN;

        static constexpr Type FEATHERING_MIN = Type(0);
        static constexpr Type FEATHERING_MAX = Type(1);
        static constexpr Type FEATHERING_NONE = FEATHERING_MIN;

        static constexpr Type ANGLE_MIN = Type(0);
        static constexpr Type ANGLE_MAX = Type(360);

        TiltShift();

        virtual ~TiltShift();

        cv::Point_<Type> get_center() const noexcept;

        void set_center(const cv::Point_<Type>& center);

        Type get_radius() const noexcept;

        void set_radius(Type radius);

        Type get_strength() const noexcept;

        void set_strength(Type strength);

        Type get_feathering() const noexcept;

        void set_feathering(Type feathering);

        Type get_angle() const noexcept;

        void set_angle(Type angle);

        MaskType get_mask_type() const noexcept;

        void set_rebuild_blurred(bool does_rebuild_blurred) noexcept;

        void set_mask_type(MaskType mask_type) noexcept;

        virtual bool apply(const cv::Mat& src, cv::Mat& dst) override;
    };
}

#endif //__FILATTI_TILT_SHIFT_HPP__
