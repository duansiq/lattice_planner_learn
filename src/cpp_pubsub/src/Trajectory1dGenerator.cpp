#include "Trajectory1dGenerator.hpp"

Trajectory1dGenerator::Trajectory1dGenerator() {}

void Trajectory1dGenerator::GenerateLateralTrajectoryBundle(
    const std::array<double, 3> &lon_init_state,
    const std::array<double, 3> &lat_init_state,
    vector<pair<vector<double>, double>> &end_conditions,
    vector<LATpath> &ptr_lat_trajectory_bundle)
{
    init_lat_state_ = lat_init_state;
    init_lon_state_ = lon_init_state;
    ptr_lat_trajectory_bundle.clear();
    for (const auto &end_condition : end_conditions)
    {
        // 计算每个终点状态对应的五次多项式参数
        const double x0 = init_lat_state_[0];
        const double dx0 = init_lat_state_[1];
        const double ddx0 = init_lat_state_[2];
        const double x1 = end_condition.first[0];
        const double dx1 = end_condition.first[1];
        const double ddx1 = end_condition.first[2];
        const double s = end_condition.second;
        // cout << "init state: " << x0 << " " << dx0 << " " << ddx0 << endl;
        ComputeCoefficients(x0, dx0, ddx0, x1, dx1, ddx1, s);
        LATpath latpath;
        latpath.a0 = coef_lat[0];
        latpath.a1 = coef_lat[1];
        latpath.a2 = coef_lat[2];
        latpath.a3 = coef_lat[3];
        latpath.a4 = coef_lat[4];
        latpath.a5 = coef_lat[5];
        latpath.smin = init_lon_state_[0];
        latpath.smax = s + init_lon_state_[0];
        latpath.end_s = s;
        latpath.end_l = end_condition.first[0];
        ptr_lat_trajectory_bundle.emplace_back(latpath);
    }
}

void Trajectory1dGenerator::ComputeCoefficients(
    const double x0, const double dx0, const double ddx0, const double x1,
    const double dx1, const double ddx1, const double p)
{
    // P是纵向的S长度
    coef_lat[0] = x0;
    coef_lat[1] = dx0;
    coef_lat[2] = ddx0 / 2.0;

    const double p2 = p * p;
    const double p3 = p * p2;

    // the direct analytical method is at least 6 times faster than using matrix
    // inversion.
    const double c0 = (x1 - 0.5 * p2 * ddx0 - dx0 * p - x0) / p3;
    const double c1 = (dx1 - ddx0 * p - dx0) / p2;
    const double c2 = (ddx1 - ddx0) / p;

    coef_lat[3] = 0.5 * (20.0 * c0 - 8.0 * c1 + c2);
    coef_lat[4] = (-15.0 * c0 + 7.0 * c1 - c2) / p;
    coef_lat[5] = (6.0 * c0 - 3.0 * c1 + 0.5 * c2) / p2;
}

void Trajectory1dGenerator::GenerateLongitudinalTrajectoryBundle(double targetspeed,
                                                                 vector<LONspeed> &ptr_lon_trajectory_bundle)
{
}

void Trajectory1dGenerator::GenerateSpeedProfilesForCruising(array<double, 3> &init_s,
                                                             vector<pair<vector<double>, double>> &lon_end_conditions,
                                                             vector<LONspeed> &ptr_lon_trajectory_bundle)
{
    ptr_lon_trajectory_bundle.clear();
    for (auto const end_condition : lon_end_conditions)
    {
        const double x0 = init_s[0];
        const double dx0 = init_s[1];
        const double ddx0 = init_s[2];
        const double dx1 = end_condition.first[0];
        const double ddx1 = end_condition.first[1];
        const double t = end_condition.second;
        ComputeCoefficients(x0, dx0, ddx0, dx1, ddx1, t);
        LONspeed lonspeed;
        lonspeed.a0 = coef_lon[0];
        lonspeed.a1 = coef_lon[1];
        lonspeed.a2 = coef_lon[2];
        lonspeed.a3 = coef_lon[3];
        lonspeed.a4 = coef_lon[4];
        lonspeed.tmin = 0;
        lonspeed.tmax = t;
        ptr_lon_trajectory_bundle.emplace_back(lonspeed);
    }
}

void Trajectory1dGenerator::ComputeCoefficients(
    const double x0, const double dx0, const double ddx0, const double dx1,
    const double ddx1, const double p)
{

    coef_lon[0] = x0;
    coef_lon[1] = dx0;
    coef_lon[2] = 0.5 * ddx0;

    double b0 = dx1 - ddx0 * p - dx0;
    double b1 = ddx1 - ddx0;

    double p2 = p * p;
    double p3 = p2 * p;

    coef_lon[3] = (3 * b0 - b1 * p) / (3 * p2);
    coef_lon[4] = (-2 * b0 + b1 * p) / (4 * p3);
}
