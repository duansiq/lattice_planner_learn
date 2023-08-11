#include "collisionchecker.hpp"
Collision_Checker::Collision_Checker() {}
Collision_Checker::~Collision_Checker() {}
bool Collision_Checker::InCollision(const vector<vector<double>> &obs_static_frenet, const LATpath &lat_trajectory,
                                    const double obs_width, const double obs_length, const std::array<double, 3> &init_s)
{
    constraint_checker.set_lat_coef(lat_trajectory);
    init_s_ = init_s;
    if (obs_static_frenet.empty())
        return true;
    else
    {
        for (const auto obs : obs_static_frenet)
        {
            double s_obs = obs[0];
            double l_obs = obs[1];
            double limit_distance = 1.5 * obs_width * obs_width;
            double evaluation_horizon = lat_trajectory.smax;
            int pointnum = 30;
            double FLAGS_trajectory_space_resolution = 4;
            for (double s = 0.0; s < evaluation_horizon; s += FLAGS_trajectory_space_resolution)
            {
                double relative_s = s - init_s_[0];
                double s_self = s;
                double l_self = constraint_checker.LATEvaluate(0, relative_s);
                double distance = (s_self - s_obs) * (s_self - s_obs) + (l_self - l_obs) * (l_self - l_obs);
                if (distance < limit_distance)
                    return false;
            }
        }
        return true;
    }
}
