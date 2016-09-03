#ifndef __FILATTI_TEMPERATURE_HPP__
#define __FILATTI_TEMPERATURE_HPP__

#include <filatti/adjustment.hpp>

namespace filatti {
    class Temperature : public Adjustment {
    private:
        double _temperature;
        cv::Mat _lut;

        void release_lut();

        void build_lut();

    protected:
        virtual bool has_effect() const noexcept override;

    public:
        const double TEMPERATURE_MIN = -0.5;
        const double TEMPERATURE_MAX = 0.5;
        const double TEMPERATURE_NONE = 0;

        Temperature();

        virtual ~Temperature();

        double get_temperature() const;

        bool set_temperature(double temperature);

        virtual bool apply(const cv::Mat& src, cv::Mat& dst) override;
    };
}

#endif //__FILATTI_TEMPERATURE_HPP__
