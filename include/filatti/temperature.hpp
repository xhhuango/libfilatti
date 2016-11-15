#ifndef __FILATTI_TEMPERATURE_HPP__
#define __FILATTI_TEMPERATURE_HPP__

#include <filatti/adjustment.hpp>
#include <filatti/dirty.hpp>
#include <filatti/synchronous.hpp>

namespace filatti {
    class Temperature : public Adjustment, public Dirty, public Synchronous {
    public:
        using Type = float;

    private:
        Type _temperature;
        cv::Mat _lut;

        void build_lut();

    public:
        static constexpr Type TEMPERATURE_MIN = Type(-0.5);
        static constexpr Type TEMPERATURE_MAX = Type(0.5);
        static constexpr Type TEMPERATURE_NONE = Type(0);

        Temperature();

        virtual ~Temperature();

        virtual bool has_effect() const noexcept override;

        Type get_temperature() const noexcept;

        void set_temperature(Type temperature);

        virtual bool apply(const cv::Mat& src, cv::Mat& dst) override;
    };
}

#endif //__FILATTI_TEMPERATURE_HPP__
