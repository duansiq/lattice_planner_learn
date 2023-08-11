#include "feasible_region.hpp"

void FeasibleRegion::Init(const array<double, 3> &init_s)
{
    init_s_ = init_s;
    double v = init_s[1];
    double max_deceleration = -constants.FLAGS_longitudinal_acceleration_lower_bound;
    t_at_zero_speed_ = v / max_deceleration;
    s_at_zero_speed_ = init_s[0] + v * v / (2.0 * max_deceleration);
}

double FeasibleRegion::SUpper(const double t) const
{
    return init_s_[0] + init_s_[1] * t +
           0.5 * constants.FLAGS_longitudinal_acceleration_upper_bound * t * t;
}

double FeasibleRegion::SLower(const double t) const
{
    if (t < t_at_zero_speed_)
    {
        return init_s_[0] + init_s_[1] * t +
               0.5 * constants.FLAGS_longitudinal_acceleration_lower_bound * t * t;
    }
    return s_at_zero_speed_;
}

double FeasibleRegion::VUpper(const double t) const
{
    return init_s_[1] + constants.FLAGS_longitudinal_acceleration_upper_bound * t;
}

double FeasibleRegion::VLower(const double t) const
{
    return t < t_at_zero_speed_
               ? init_s_[1] + constants.FLAGS_longitudinal_acceleration_lower_bound * t
               : 0.0;
}

double FeasibleRegion::TLower(const double s) const
{
    double delta_s = s - init_s_[0];
    double v = init_s_[1];
    double a = constants.FLAGS_longitudinal_acceleration_upper_bound;
    double t = (sqrt(v * v + 2.0 * a * delta_s) - v) / a;
    return t;
}