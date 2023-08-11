#include "constraint_checker1d.hpp"
Constraint_Checker1d::Constraint_Checker1d()
{
}
Constraint_Checker1d::~Constraint_Checker1d() {}

inline bool fuzzy_within(const double v, const double lower, const double upper,
                         const double e = 1.0e-4)
{
    return v > lower - e && v < upper + e;
}

void Constraint_Checker1d::set_lon_coef(const LONspeed &lon_trajectory)
{
    lon_coef_ = lon_trajectory;
}
void Constraint_Checker1d::set_lat_coef(const LATpath &lat_trajectory)
{
    lat_coef_ = lat_trajectory;
}

bool Constraint_Checker1d::IsValidLongitudinalTrajectory(
    const LONspeed &lon_trajectory)
{
    double t = 0.0;
    while (t < lon_trajectory.tmax)
    {
        double v = LONEvaluate(1, t); // evaluate_v
        if (!fuzzy_within(v, constants.FLAGS_speed_lower_bound, constants.FLAGS_speed_upper_bound))
        {
            return false;
        }

        double a = LONEvaluate(2, t); // evaluate_a
        if (!fuzzy_within(a, constants.FLAGS_longitudinal_acceleration_lower_bound,
                          constants.FLAGS_longitudinal_acceleration_upper_bound))
        {
            return false;
        }

        double j = LONEvaluate(3, t); // evaluate_jerk
        if (!fuzzy_within(j, constants.FLAGS_longitudinal_jerk_lower_bound,
                          constants.FLAGS_longitudinal_jerk_upper_bound))
        {
            return false;
        }
        t += constants.FLAGS_trajectory_time_resolution;
    }
    return true;
}

bool Constraint_Checker1d::IsValidLateralTrajectory(
    const LATpath &lat_trajectory, const LONspeed &lon_trajectory)
{
    double t = 0.0;
    while (t < lon_trajectory.tmax)
    {
        double s = LONEvaluate(0, t);
        double dd_ds = LATEvaluate(1, s);
        double ds_dt = LONEvaluate(1, t);

        double d2d_ds2 = LATEvaluate(2, s);
        double d2s_dt2 = LONEvaluate(2, t);

        double a = 0.0;
        if (s < lat_trajectory.smax)
        {
            a = d2d_ds2 * ds_dt * ds_dt + dd_ds * d2s_dt2;
        }

        if (!fuzzy_within(a, -constants.FLAGS_lateral_acceleration_bound,
                          constants.FLAGS_lateral_acceleration_bound))
        {
            return false;
        }

        // this is not accurate, just an approximation...
        double j = 0.0;
        if (s < lat_trajectory.smax)
        {
            j = LATEvaluate(3, s) * LONEvaluate(3, t);
        }

        if (!fuzzy_within(j, -constants.FLAGS_lateral_jerk_bound, constants.FLAGS_lateral_jerk_bound))
        {
            return false;
        }
        t += constants.FLAGS_trajectory_time_resolution;
    }
    return true;
}

// 已经有四次多项式的系数，直接将t代入方程求解 s v a jerk;
double Constraint_Checker1d::LONEvaluate(const std::uint32_t order,
                                         const double p) const
{
    switch (order)
    {
    case 0:
    {

        return (((lon_coef_.a4 * p + lon_coef_.a3) * p + lon_coef_.a2) * p + lon_coef_.a1) * p +
               lon_coef_.a0;
    }
    case 1:
    {
        return ((4.0 * lon_coef_.a4 * p + 3.0 * lon_coef_.a3) * p + 2.0 * lon_coef_.a2) * p +
               lon_coef_.a1;
    }
    case 2:
    {
        return (12.0 * lon_coef_.a4 * p + 6.0 * lon_coef_.a3) * p + 2.0 * lon_coef_.a2;
    }
    case 3:
    {
        return 24.0 * lon_coef_.a4 * p + 6.0 * lon_coef_.a3;
    }
    case 4:
    {
        return 24.0 * lon_coef_.a4;
    }
    default:
        return 0.0;
    }
}

// 五次多项式的插值计算，输入的是纵向的S ，输出的是对应的横向的的参数
double Constraint_Checker1d::LATEvaluate(const uint32_t order,
                                         const double p) const
{
    switch (order)
    {
    case 0:
    {
        return ((((lat_coef_.a5 * p + lat_coef_.a4) * p + lat_coef_.a3) * p + lat_coef_.a2) * p +
                lat_coef_.a1) *
                   p +
               lat_coef_.a0;
    }
    case 1:
    {
        return (((5.0 * lat_coef_.a5 * p + 4.0 * lat_coef_.a4) * p + 3.0 * lat_coef_.a3) * p +
                2.0 * lat_coef_.a2) *
                   p +
               lat_coef_.a1;
    }
    case 2:
    {
        return (((20.0 * lat_coef_.a5 * p + 12.0 * lat_coef_.a4) * p) + 6.0 * lat_coef_.a3) *
                   p +
               2.0 * lat_coef_.a2;
    }
    case 3:
    {
        return (60.0 * lat_coef_.a5 * p + 24.0 * lat_coef_.a4) * p + 6.0 * lat_coef_.a3;
    }
    case 4:
    {
        return 120.0 * lat_coef_.a5 * p + 24.0 * lat_coef_.a4;
    }
    case 5:
    {
        return 120.0 * lat_coef_.a5;
    }
    default:
        return 0.0;
    }
}