#ifndef __FILATTI_CURVES_HPP__
#define __FILATTI_CURVES_HPP__

#include <filatti/adjustment.hpp>

namespace filatti {
    class Curves : public Adjustment {
    private:
        std::pair<std::vector<uchar>, std::vector<uchar>> _value_points;
        std::pair<std::vector<uchar>, std::vector<uchar>> _blue_points;
        std::pair<std::vector<uchar>, std::vector<uchar>> _green_points;
        std::pair<std::vector<uchar>, std::vector<uchar>> _red_points;

        cv::Mat _lut;

        inline bool check_input_points(const std::vector<uchar>& from, const std::vector<uchar>& to) const;

        std::vector<uchar> get_curves(const std::pair<std::vector<uchar>, std::vector<uchar>>& points) const;

        void release_lut();

        void build_lut();

    protected:
        virtual bool has_effect() const override;

    public:
        const int POINTS_SIZE_MIN = 2;
        const int POINTS_SIZE_MAX = 256;

        const std::vector<uchar> POINTS_NONE{0, 255};

        Curves();

        virtual ~Curves();

        /**
         * Value
         */
        bool set_value_points(std::vector<uchar> from, std::vector<uchar> to);

        std::pair<std::vector<uchar>, std::vector<uchar>> get_value_points() const;

        std::vector<uchar> get_value_curves() const;

        /**
         * Blue
         */
        bool set_blue_points(std::vector<uchar> from, std::vector<uchar> to);

        std::pair<std::vector<uchar>, std::vector<uchar>> get_blue_points() const;

        std::vector<uchar> get_blue_curves() const;

        /**
         * Green
         */
        bool set_green_points(std::vector<uchar> from, std::vector<uchar> to);

        std::pair<std::vector<uchar>, std::vector<uchar>> get_green_points() const;

        std::vector<uchar> get_green_curves() const;

        /**
         * Red
         */
        bool set_red_points(std::vector<uchar> from, std::vector<uchar> to);

        std::pair<std::vector<uchar>, std::vector<uchar>> get_red_points() const;

        std::vector<uchar> get_red_curves() const;

        virtual bool apply(const cv::Mat& src, cv::Mat& dst) override;
    };
};

#endif //__FILATTI_CURVES_HPP__
