#ifndef _END_CONDITION_SAMPLER_
#define _END_CONDITION_SAMPLER_
#include <array>
#include <vector>
#include "STpoint.hpp"
#include "feasible_region.hpp"
#include "constans.hpp"
#include "path_time_graph.hpp"
#include "STBoundaryPoint.hpp"

using  namespace std;
struct SamplePoint {
  double ref_v;
  double s;
  double t;
};
class EndConditionSampler
{

public:

    EndConditionSampler(){}
    ~EndConditionSampler(){}

    void Init( const array<double, 3>& init_s, const array<double, 3>& init_d)
    {
        init_s_ = init_s;
        init_d_ = init_d;
        feasibleregion.Init(init_s);
        // pathtimegraph.Init();
    }

    vector<pair<vector<double>, double>> 
    SampleLatEndConditions() const;

    vector<pair<vector<double>, double>>
    SampleLonEndConditionsForCruising(const double ref_cruise_speed) const;

    vector<pair<vector<double>, double>>
    SampleLonEndConditionsForStopping(const double ref_stop_point) const;

    vector<pair<vector<double>, double>>
    SampleLonEndConditionsForLaneChange(const vector<STBoundaryPoint>& stboundarypoint , const double cruise_speed) const;


private:
// std::vector<SamplePoint> QueryPathTimeObstacleSamplePoints() const;

// void QueryFollowPathTimePoints(
//     const apollo::common::VehicleConfig& vehicle_config,
//     const std::string& obstacle_id,
//     std::vector<SamplePoint>* sample_points) const;

// void QueryOvertakePathTimePoints(
//     const apollo::common::VehicleConfig& vehicle_config,
//     const std::string& obstacle_id,
//     std::vector<SamplePoint>* sample_points) const;
private:
    FeasibleRegion feasibleregion;
    Constans constans;
    PathTimeGraph pathtimegraph;
    array<double, 3> init_s_;
    array<double, 3> init_d_;
};

#endif