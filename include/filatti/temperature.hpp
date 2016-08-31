#ifndef __FILATTI_TEMPERATURE_HPP__
#define __FILATTI_TEMPERATURE_HPP__

#include <filatti/adjustment.hpp>

namespace filatti {
    class Temperature : public Adjustment {
    private:
        unsigned int _kelvin;
        double _strength;
        cv::Mat _lut;

        cv::Vec3b kelvin_to_color(unsigned int kelvin) const;

        uchar kelvin_to_b(unsigned int kelvin) const;

        uchar kelvin_to_g(unsigned int kelvin) const;

        uchar kelvin_to_r(unsigned int kelvin) const;

        void release_lut();

        void build_lut(const cv::Vec3b& color);

    protected:
        virtual bool has_effect() const override;

    public:
        const unsigned int KELVIN_NONE = 6600;
        const unsigned int KELVIN_MIN = 1000;
        const unsigned int KELVIN_MAX = 40000;

        const double STRENGTH_MIN = 0;
        const double STRENGTH_MAX = 1;
        const double STRENGTH_NONE = STRENGTH_MIN;

        Temperature();

        virtual ~Temperature();

        unsigned int get_kelvin() const;

        bool set_kelvin(unsigned int kelvin);

        double get_strength() const;

        bool set_strength(double strength);

        virtual bool apply(const cv::Mat& src, cv::Mat& dst) override;
    };
}

#endif //__FILATTI_TEMPERATURE_HPP__
