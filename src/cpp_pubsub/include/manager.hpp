#ifndef _MANAGER_
#define _MANAGER_
#include <chrono>
#include <vector>
#include <memory>
#include <array>
#include <math.h>
#include <string.h>
#include "rclcpp/rclcpp.hpp"
#include "obstacle.hpp"
#include "path_matcher.hpp"
#include "TrajectoryPoint.hpp"
#include "cartesian_frenet_converter.hpp"
#include "latpath1d.hpp"
#include "lonspeed1d.hpp"
#include "Trajectory1dGenerator.hpp"
#include "referenceline.hpp"
#include "test.hpp"
#include "pathpoint.hpp"
#include "trajectory_evaluator.hpp"
#include "collisionchecker.hpp"
#include "plotfigure.hpp"
#include "constraint_checker1d.hpp"
#include "end_condition_sampler.hpp"
#include "obs.hpp"
#include "path_time_graph.hpp"
#include "STBoundaryPoint.hpp"
#include "constans.hpp"

using namespace std::chrono;
using namespace std;

class Manage_Planner
{
public:
    Manage_Planner();
    ~Manage_Planner();
    void test()
    {
        cout << " OKOKOKOK";
        test_.test_include();
    };
    // 载入数据
    void load_referenceline(vector<PathPoint> &ref_points) { referenceline_points = ref_points; };
    void load_obstacle(){};
    void load_selfpose(){};

    vector<PathPoint> ToDiscretizedReferenceLine(const vector<PathPoint> &ref_points);
    void ComputeInitFrenetState(const PathPoint &matched_point,
                                const TrajectoryPoint &cartesian_state,
                                std::array<double, 3> *ptr_s,
                                std::array<double, 3> *ptr_d);
    void ComputeObsFrenetState(const PathPoint &matched_point,
                               const double x, const double y,
                               double *ptr_s, double *ptr_d);

    void planner();

    vector<PathPoint> get_referenceline_points() { return referenceline_points; };
    vector<TrajectoryPoint> get_latpath() { return latpath_gcs; };
    vector<vector<double>> get_obstacle() { return obs_static_gcs_; };
    void writetxt();

private:
    /* data */
    Test test_;
    Cartesian_Frenet_Converter cartesianfrenetconverter;
    Path_Matcher pathmatcher;
    Obstacle obstacle;
    Trajectory1dGenerator trajectory1dgenerator;
    ReferenceLine referenceline;
    PathTimeGraph pathtimegraph;
    Constans constans;

    vector<PathPoint> referenceline_points;
    vector<TrajectoryPoint> latpath_gcs;
    vector<vector<double>> obs_static_gcs_;
    Trajectory_Evaluator trajectory_evaluator;
    Collision_Checker collision_checker;
    LATpath latpath;
    LATpath lat_trajectory;
    Constraint_Checker1d latpointcal;

    EndConditionSampler endconditionsampler;
   

    // 可视化数据可视填充

    // PlotFigure plotfigure;
};

#endif