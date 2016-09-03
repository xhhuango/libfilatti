#ifndef __FILATTI_CURVES_HPP__
#define __FILATTI_CURVES_HPP__

#include <filatti/adjustment.hpp>
#include <filatti/dirty.hpp>
#include <filatti/synchronous.hpp>

namespace filatti {
    class Curves : public Adjustment, public Dirty, public Synchronous {
    private:
        std::pair<std::vector<uchar>, std::vector<uchar>> _value_points;
        std::pair<std::vector<uchar>, std::vector<uchar>> _blue_points;
        std::pair<std::vector<uchar>, std::vector<uchar>> _green_points;
        std::pair<std::vector<uchar>, std::vector<uchar>> _red_points;

        cv::Mat _lut;

        inline void check_input_points(const std::vector<uchar>& from, const std::vector<uchar>& to) const;

        void build_lut();

    protected:
        virtual bool has_effect() const noexcept override;

    public:
        const int POINTS_SIZE_MIN = 2;
        const int POINTS_SIZE_MAX = 256;

        const std::vector<uchar> POINTS_NONE{0, 255};

        Curves();

        virtual ~Curves();

        /**
         * Value
         */
        void set_value_points(const std::vector<uchar>& from, const std::vector<uchar>& to);

        std::pair<std::vector<uchar>, std::vector<uchar>> get_value_points() const noexcept;

        std::vector<uchar> get_value_curves() const noexcept;

        /**
         * Blue
         */
        void set_blue_points(const std::vector<uchar>& from, const std::vector<uchar>& to);

        std::pair<std::vector<uchar>, std::vector<uchar>> get_blue_points() const noexcept;

        std::vector<uchar> get_blue_curves() const noexcept;

        /**
         * Green
         */
        void set_green_points(const std::vector<uchar>& from, const std::vector<uchar>& to);

        std::pair<std::vector<uchar>, std::vector<uchar>> get_green_points() const noexcept;

        std::vector<uchar> get_green_curves() const noexcept;

        /**
         * Red
         */
        void set_red_points(const std::vector<uchar>& from, const std::vector<uchar>& to);

        std::pair<std::vector<uchar>, std::vector<uchar>> get_red_points() const noexcept;

        std::vector<uchar> get_red_curves() const noexcept;

        virtual bool apply(const cv::Mat& src, cv::Mat& dst) override;
    };
};

#endif //__FILATTI_CURVES_HPP__
