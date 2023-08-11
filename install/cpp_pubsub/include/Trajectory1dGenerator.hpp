#ifndef _TRAJECTORYGENERATOR_
#define _TRAJECTORYGENERATOR_
#include <array>
#include <memory>
#include <vector>
#include <algorithm>
#include "latpath1d.hpp"
#include "lonspeed1d.hpp"
#include <iostream>
#include <ostream>

using namespace std;

class Trajectory1dGenerator
{
public:
    Trajectory1dGenerator();
    virtual ~Trajectory1dGenerator() = default;

    void GenerateLateralTrajectoryBundle(
        const std::array<double, 3> &lon_init_state,
        const std::array<double, 3> &lat_init_state,
        vector<pair<vector<double>, double>> &end_conditions,
        vector<LATpath> &ptr_lat_trajectory_bundle);

    void GenerateLongitudinalTrajectoryBundle(const double targetspeed,
                                              vector<LONspeed> &ptr_lon_trajectory_bundle);

    void ComputeCoefficients(const double x0, const double dx0, const double ddx0,
                             const double x1, const double dx1, const double ddx1, const double p);
    void ComputeCoefficients(const double x0, const double dx0, const double ddx0,
                             const double dx1, const double ddx1, const double p);
    void GenerateSpeedProfilesForCruising(array<double, 3> &init_s,
                                          vector<pair<vector<double>, double>> &lon_end_conditions,
                                          vector<LONspeed> &ptr_lon_trajectory_bundle);

private:
    array<double, 3> init_lon_state_;
    array<double, 3> init_lat_state_;
    array<double, 6> coef_lat{{0.0, 0.0, 0.0, 0.0, 0.0, 0.0}};
    array<double, 5> coef_lon{{0.0, 0.0, 0.0, 0.0, 0.0}};
    // EndConditionSampler end_condition_sampler_;
    // std::shared_ptr<PathTimeGraph> ptr_path_time_graph_;
};

#endif