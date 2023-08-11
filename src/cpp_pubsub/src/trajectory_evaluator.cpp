#include "trajectory_evaluator.hpp"

Trajectory_Evaluator::Trajectory_Evaluator() {}
Trajectory_Evaluator::~Trajectory_Evaluator() {}
// 主函数
// 主函数
void Trajectory_Evaluator::TrajectoryEvaluator(
    const std::array<double, 3> &init_s,
    const std::vector<LONspeed> &lon_trajectories,
    const std::vector<LATpath> &lat_trajectories,
    const std::vector<PathPoint> &reference_line)
{
    while (!cost_queue_.empty())
    {
        cost_queue_.pop();
    }
    init_s_ = init_s;
    for (const auto &lat_trajectory : lat_trajectories)
    {

        int pointnum = 300;
        // 曲线参数设置
        constraint_checker1d.set_lat_coef(lat_trajectory);
        // s离散取值
        double evaluation_horizon = lat_trajectory.smax;
        vector<double> s_values;
        double FLAGS_trajectory_space_resolution = evaluation_horizon / pointnum;
        for (double s = 0.0; s < evaluation_horizon;
             s += FLAGS_trajectory_space_resolution)
        {
            s_values.emplace_back(s);
        }

        double lat_offset_cost = LatOffsetCost(lat_trajectory, s_values);
        double lat_comfort_cost = LatComfortCost(s_values, lat_trajectory);
        double cost = lat_offset_cost * constants.FLAGS_weight_lat_offset +
                      lat_comfort_cost * constants.FLAGS_weight_lat_comfort;

        cost_queue_.emplace(cost, lat_trajectory);
    }
}

double Trajectory_Evaluator::LatComfortCost(const vector<double> &s_values,
                                            const LATpath &lat_trajectory) const
{
    double max_cost = 0.0;
    for (const auto &s : s_values)
    {
        double l_prime = constraint_checker1d.LATEvaluate(1, s);
        double l_primeprime = constraint_checker1d.LATEvaluate(2, s);
        double l_primeprimeprime = constraint_checker1d.LATEvaluate(3, s);

        double cost = constants.FLAGS_l_prime_wight * l_prime * l_prime +
                      constants.FLAGS_l_primeprime_wight * l_primeprime * l_primeprime +
                      constants.FLAGS_l_primeprimeprime_wight * l_primeprimeprime * l_primeprimeprime;
        max_cost += cost;
    }
    return max_cost;
}

double Trajectory_Evaluator::LatOffsetCost(
    const LATpath &lat_trajectory,
    const std::vector<double> &s_values) const
{
    double lat_offset_start = constraint_checker1d.LATEvaluate(0, 0.0);
    double cost_sqr_sum = 0.0;
    double cost_abs_sum = 0.0;
    for (const auto &s : s_values)
    {
        double lat_offset = constraint_checker1d.LATEvaluate(0, s); // 横向偏移
        double cost = lat_offset / constants.FLAGS_lat_offset_bound;
        // 判断和起点的l是否在同一边
        if (lat_offset * lat_offset_start < 0.0)
        {
            cost_sqr_sum += cost * cost * constants.FLAGS_weight_opposite_side_offset;
            cost_abs_sum += fabs(cost) * constants.FLAGS_weight_opposite_side_offset;
        }
        else
        {
            cost_sqr_sum += cost * cost * constants.FLAGS_weight_same_side_offset;
            cost_abs_sum += fabs(cost) * constants.FLAGS_weight_same_side_offset;
        }
    }
    return cost_sqr_sum / (cost_abs_sum + constants.FLAGS_numerical_epsilon);
}

// void Trajectory_Evaluator::TrajectoryEvaluator(
//     const std::array<double, 3> &init_s,
//     const std::vector<LONspeed> &lon_trajectories,
//     const std::vector<LATpath> &lat_trajectories,
//     const std::vector<PathPoint> &reference_line)
// {
//     init_s_ = init_s;

//     for (const auto &lat_trajectory : lat_trajectories)
//     {
//         int pointnum = 30;
//         // 曲线参数设置
//         constraint_checker1d.set_lat_coef(lat_trajectory);
//         // s离散取值
//         double evaluation_horizon = lat_trajectory.smax;
//         std::vector<double> s_values;
//         double FLAGS_trajectory_space_resolution = evaluation_horizon / pointnum;
//         for (double s = 0.0; s < evaluation_horizon;
//              s += FLAGS_trajectory_space_resolution)
//         {
//             s_values.emplace_back(s);
//         }

//         double lat_offset_cost = LatOffsetCost(lat_trajectory, s_values);

//         double lat_comfort_cost = LatComfortCost(lon_trajectory, lat_trajectory);

//     }

//     // for (const auto &lon_trajectory : lon_trajectories)
//     // {
//     //     constraint_checker1d.set_lon_coef(lon_trajectory);
//     //     double lon_end_s = constraint_checker1d.LONEvaluate(0, lon_trajectory.tmax);
//     //     // 针对停车点的判断
//     //     //  if (init_s[0] < stop_point &&
//     //     //      lon_end_s + FLAGS_lattice_stop_buffer > stop_point)
//     //     //  {
//     //     //      continue;
//     //     //  }
//     //     if (!constraint_checker1d.IsValidLongitudinalTrajectory(lon_trajectory))
//     //     {
//     //         continue;
//     //     }
//     // for (const auto &lat_trajectory : lat_trajectories)
//     // {
//     //     constraint_checker1d.set_lat_coef(lat_trajectory);
//     //     // The validity of the code needs to be verified.
//     //     if (!constraint_checker1d.IsValidLateralTrajectory(lat_trajectory,
//     //                                                     lon_trajectory))
//     //     {
//     //         continue;
//     //     }

//     //     double cost = Evaluate(lon_trajectory, lat_trajectory);
//     //     cost_queue_.emplace(Trajectory1dPair(lon_trajectory, lat_trajectory),
//     //                         cost);
//     // }
//     // }
// }

// double Trajectory_Evaluator::Evaluate(const LONspeed &lon_trajectory,
//                                       const LATpath &lat_trajectory,
//                                       std::vector<double> *cost_components = nullptr) const
// {
// Costs:
// 1. Cost of missing the objective, e.g., cruise, stop, etc.
// 2. Cost of longitudinal jerk
// 3. Cost of longitudinal collision
// 4. Cost of lateral offsets
// 5. Cost of lateral comfort

// Longitudinal costs
// 目标速度的cost，暂时不考虑
// double lon_objective_cost =
// LonObjectiveCost(lon_trajectory, planning_target, reference_s_dot_);

// double lon_jerk_cost = LonComfortCost(lon_trajectory);

// double lon_collision_cost = LonCollisionCost(lon_trajectory);

// double centripetal_acc_cost = CentripetalAccelerationCost(lon_trajectory);

// decides the longitudinal evaluation horizon for lateral trajectories.
// 纵向规划的s作为横向规划评价的s
// double evaluation_horizon = min(constants.FLAGS_speed_lon_decision_horizon,
//                                 constraint_checker1d.LONEvaluate(0, lon_trajectory.tmax));
// std::vector<double> s_values;
// for (double s = 0.0; s < evaluation_horizon;
//      s += constants.FLAGS_trajectory_space_resolution)
// {
//     s_values.emplace_back(s);
// }

// // Lateral costs
// double lat_offset_cost = LatOffsetCost(lat_trajectory, s_values);

// double lat_comfort_cost = LatComfortCost(lon_trajectory, lat_trajectory);

// if (cost_components != nullptr)
// {
//     // cost_components->emplace_back(lon_objective_cost);
//     cost_components->emplace_back(lon_jerk_cost);
//     cost_components->emplace_back(lon_collision_cost);
//     cost_components->emplace_back(lat_offset_cost);
// }

// return lon_objective_cost * constants.FLAGS_weight_lon_objective +
//  lon_jerk_cost * constants.FLAGS_weight_lon_jerk +
//        lon_collision_cost * constants.FLAGS_weight_lon_collision +
//        centripetal_acc_cost * constants.FLAGS_weight_centripetal_acceleration +
//     return lat_offset_cost * constants.FLAGS_weight_lat_offset +
//            lat_comfort_cost * constants.FLAGS_weight_lat_comfort;
// }

// double Trajectory_Evaluator::LatOffsetCost(
//     const LATpath &lat_trajectory,
//     const std::vector<double> &s_values) const
// {
//     double lat_offset_start = constraint_checker1d.LATEvaluate(0, 0.0);
//     double cost_sqr_sum = 0.0;
//     double cost_abs_sum = 0.0;
//     for (const auto &s : s_values)
//     {
//         double lat_offset = constraint_checker1d.LATEvaluate(0, s); // 横向pinyil
//         double cost = lat_offset / constants.FLAGS_lat_offset_bound;
//         // 判断和起点的l是否在同一边
//         if (lat_offset * lat_offset_start < 0.0)
//         {
//             cost_sqr_sum += cost * cost * constants.FLAGS_weight_opposite_side_offset;
//             cost_abs_sum += std::fabs(cost) * constants.FLAGS_weight_opposite_side_offset;
//         }
//         else
//         {
//             cost_sqr_sum += cost * cost * constants.FLAGS_weight_same_side_offset;
//             cost_abs_sum += std::fabs(cost) * constants.FLAGS_weight_same_side_offset;
//         }
//     }
//     return cost_sqr_sum / (cost_abs_sum + constants.FLAGS_numerical_epsilon);
// }

// double Trajectory_Evaluator::LatComfortCost(
//     const LONspeed &lon_trajector,
//     const LATpath &lat_trajectory) const
// {
//     double max_cost = 0.0;
//     for (double t = 0.0; t < constants.FLAGS_trajectory_time_length;
//          t += constants.FLAGS_trajectory_time_resolution)
//     {
//         double s = constraint_checker1d.LONEvaluate(0, t);
//         double s_dot = constraint_checker1d.LONEvaluate(1, t);
//         double s_dotdot = constraint_checker1d.LONEvaluate(2, t);

//         double relative_s = s - init_s_[0];
//         double l_prime = constraint_checker1d.LATEvaluate(1, relative_s);
//         double l_primeprime = constraint_checker1d.LATEvaluate(2, relative_s);
//         double cost = l_primeprime * s_dot * s_dot + l_prime * s_dotdot;
//         max_cost = std::max(max_cost, std::fabs(cost));
//     }
//     return max_cost;
// }

// double Trajectory_Evaluator::LonComfortCost(
//     const LONspeed &lon_trajectory) const
// {
//     double cost_sqr_sum = 0.0;
//     double cost_abs_sum = 0.0;
//     for (double t = 0.0; t < constants.FLAGS_trajectory_time_length;
//          t += constants.FLAGS_trajectory_time_resolution)
//     {
//         double jerk = constraint_checker1d.LONEvaluate(3, t);
//         double cost = jerk / constants.FLAGS_longitudinal_jerk_upper_bound;
//         cost_sqr_sum += cost * cost;
//         cost_abs_sum += fabs(cost);
//     }
//     return cost_sqr_sum / (cost_abs_sum + constants.FLAGS_numerical_epsilon);
// }

// double TrajectoryEvaluator::LonObjectiveCost(
//     const PtrTrajectory1d& lon_trajectory,
//     const PlanningTarget& planning_target,
//     const std::vector<double>& ref_s_dots) const {
//   double t_max = lon_trajectory->ParamLength();
//   double dist_s =
//       lon_trajectory->Evaluate(0, t_max) - lon_trajectory->Evaluate(0, 0.0);

//   double speed_cost_sqr_sum = 0.0;
//   double speed_cost_weight_sum = 0.0;
//   for (size_t i = 0; i < ref_s_dots.size(); ++i) {
//     double t = static_cast<double>(i) * FLAGS_trajectory_time_resolution;
//     double cost = ref_s_dots[i] - lon_trajectory->Evaluate(1, t);
//     speed_cost_sqr_sum += t * t * std::fabs(cost);
//     speed_cost_weight_sum += t * t;
//   }
//   double speed_cost =
//       speed_cost_sqr_sum / (speed_cost_weight_sum + FLAGS_numerical_epsilon);
//   double dist_travelled_cost = 1.0 / (1.0 + dist_s);
//   return (speed_cost * FLAGS_weight_target_speed +
//           dist_travelled_cost * FLAGS_weight_dist_travelled) /
//          (FLAGS_weight_target_speed + FLAGS_weight_dist_travelled);
// }
