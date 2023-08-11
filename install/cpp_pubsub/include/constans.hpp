#ifndef _CONSTANTS_
#define _CONSTANTS_

typedef struct Constans
{
    //车辆参数 车道参数
    const double FLAGS_road_width = 3.75;
    const double FLAGS_vehicle_config_width = 2.;
    const double FLAGS_vehicle_config_length = 5.;
    
    const double FLAGS_lateral_acceleration_bound = 0;
    const double FLAGS_lateral_jerk_bound = 0;
    const double FLAGS_longitudinal_jerk_lower_bound = 0;
    const double FLAGS_longitudinal_jerk_upper_bound = 0;
    const double FLAGS_longitudinal_acceleration_lower_bound = 0.;
    const double FLAGS_longitudinal_acceleration_upper_bound = 0;
    const double FLAGS_speed_lower_bound = 0;
    const double FLAGS_speed_upper_bound = 0;

    const double FLAGS_trajectory_time_length = 8.0;
    const double FLAGS_trajectory_time_resolution = 0.1;
    const double FLAGS_trajectory_sample_s_num = 6;

    const double FLAGS_polynomial_minimal_param = 0.01;

    const int FLAGS_num_velocity_sample = 3;
    const double FLAGS_min_velocity_sample_gap = 5;

    const double FLAGS_weight_lon_objective = 0;
    const double FLAGS_weight_lon_jerk = 0;
    const double FLAGS_weight_lon_collision = 0;
    const double FLAGS_weight_centripetal_acceleration = 0;

    const double FLAGS_speed_lon_decision_horizon = 200; //纵向规划长度
    const double FLAGS_trajectory_space_resolution = 0;
    const double FLAGS_numerical_epsilon = 0.01;

    // 路径规划
    const double FLAGS_l_prime_wight = 100.;
    const double FLAGS_l_primeprime_wight = 2000.;
    const double FLAGS_l_primeprimeprime_wight = 100.;

    const double FLAGS_weight_opposite_side_offset = 100.;
    const double FLAGS_weight_same_side_offset = 50.;
    const double FLAGS_lat_offset_bound = 4.;
    const double FLAGS_weight_lat_offset = 2000;
    const double FLAGS_weight_lat_comfort = 100;

} Constans;

#endif