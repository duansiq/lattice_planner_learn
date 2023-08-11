#ifndef _TRAJECTORY_EVALUATOR_
#define _TRAJECTORY_EVALUATOR_
#include <array>
#include <vector>
#include <math.h>
#include "latpath1d.hpp"
#include "lonspeed1d.hpp"
#include "pathpoint.hpp"
#include "constraint_checker1d.hpp"
#include "constans.hpp"
#include <queue>

using namespace std;
class Trajectory_Evaluator
{

public:
    Trajectory_Evaluator();
    ~Trajectory_Evaluator();
    // 主函数
    void TrajectoryEvaluator(
        const std::array<double, 3> &init_s,
        const std::vector<LONspeed> &lon_trajectories,
        const std::vector<LATpath> &lat_trajectories,
        const std::vector<PathPoint> &reference_line);

    bool has_more_trajectory() const
    {
        return !cost_queue_.empty();
    }
    size_t num_of_trajectory() const
    {
        return cost_queue_.size();
    }

    LATpath gettop_trajectory()
    {
        auto top = cost_queue_.top();
        cost_queue_.pop();
        return top.second;
    }

    double gettop_cost()
    {
        return cost_queue_.top().first;
    }

private:
    double Evaluate(const LONspeed &lon_trajectory,
                    const LATpath &lat_trajectory,
                    std::vector<double> *cost_components = nullptr) const;
    Constraint_Checker1d constraint_checker1d;
    Constans constants;
    array<double, 3> init_s_;

    double LatOffsetCost(const LATpath &lat_trajectory,
                                               const std::vector<double> &s_values) const;

    double LatComfortCost(const vector<double> &s_values,
                                                const LATpath &lat_trajectory) const;

    struct CostComparator
        : public std::binary_function<const pair<double, LATpath> &, const pair<double, LATpath> &, bool>
    {
        bool operator()(const pair<double, LATpath> &left, const pair<double, LATpath> &right) const
        {
            return left.first > right.first;
        }
    };

    priority_queue<pair<double, LATpath>, vector<pair<double, LATpath>>, CostComparator> cost_queue_;

    // double LonComfortCost(const LONspeed &lon_trajectory) const;

    // double LonCollisionCost(const std::shared_ptr<Curve1d> &lon_trajectory) const;

    // double LonObjectiveCost(const std::shared_ptr<Curve1d> &lon_trajectory,
    //                         const PlanningTarget &planning_target,
    //                         const std::vector<double> &ref_s_dot) const;
};

#endif