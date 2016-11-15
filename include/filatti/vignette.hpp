#ifndef __FILATTI_VIGNETTE_HPP__
#define __FILATTI_VIGNETTE_HPP__

#include <filatti/adjustment.hpp>
#include <filatti/dirty.hpp>
#include <filatti/synchronous.hpp>

namespace filatti {
    class Vignette : public Adjustment, public Dirty, public Synchronous {
    public:
        using Type = float;

    private:
        cv::Point_<Type> _center;
        Type _radius;
        Type _strength;
        Type _feathering;
        cv::Scalar_<uchar> _color;
        bool _fit_to_image;

        cv::Mat _mask;

        void create_mask(const cv::Size& size);

        void blend(const cv::Mat& src, cv::Mat& dst) const;

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

        Vignette();

        virtual ~Vignette();

        virtual bool has_effect() const noexcept override;

        cv::Point_<Type> get_center() const noexcept;

        void set_center(const cv::Point_<Type>& center);

        Type get_radius() const noexcept;

        void set_radius(Type radius);

        Type get_strength() const noexcept;

        void set_strength(Type strength);

        Type get_feathering() const noexcept;

        void set_feathering(Type feathering);

        cv::Scalar_<uchar> get_color() const noexcept;

        void set_color(const cv::Scalar_<uchar>& color);

        bool is_fit_to_image() const noexcept;

        void set_fit_to_image(bool fit_to_image) noexcept;

        virtual bool apply(const cv::Mat& src, cv::Mat& dst) override;
    };
}

#endif //__FILATTI_VIGNETTE_HPP__
