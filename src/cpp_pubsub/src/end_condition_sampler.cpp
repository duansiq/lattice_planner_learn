#include "end_condition_sampler.hpp"
#include <algorithm>

// EndConditionSampler::EndConditionSampler() {}
// EndConditionSampler::~EndConditionSampler(){}

vector<SamplePoint>
QueryPathTimeObstacleSamplePoints(const vector<STBoundaryPoint> &stboundarypoint, double vehicle_length,
                                  double sample_s_num, const double cruise_speed);

vector<pair<vector<double>, double>> EndConditionSampler::SampleLatEndConditions() const
{
    vector<double> end_d_candidates = {-0.5, 0, 0.5};
    vector<double> end_s_candidates = {20, 40, 60, 80};
    vector<pair<vector<double>, double>> lat_end_conditions;
    for (const auto &s : end_s_candidates)
    {
        for (const auto &d : end_d_candidates)
        {
            vector<double> end_d_state = {d, 0.0, 0.0};
            lat_end_conditions.emplace_back(end_d_state, s);
        }
    }
    return lat_end_conditions;
}

vector<pair<vector<double>, double>>
EndConditionSampler::SampleLonEndConditionsForCruising(const double ref_cruise_speed) const
{
    vector<double> end_ds_candidates;
    vector<double> end_t_candidates;
    end_ds_candidates = {0, 1. / 6. * ref_cruise_speed, 2. / 6. * ref_cruise_speed, 3. / 6. * ref_cruise_speed,
                         4. / 6. * ref_cruise_speed, 5. / 6. * ref_cruise_speed, ref_cruise_speed};
    end_t_candidates = {1, 2, 3, 4, 5, 6, 7, 8};

    vector<pair<vector<double>, double>> lon_end_conditions;
    for (const auto &t : end_t_candidates)
    {
        for (const auto &ds : end_ds_candidates)
        {
            vector<double> end_s_state = {ds, 0.0};
            lon_end_conditions.emplace_back(end_s_state, t);
        }
    }
    return lon_end_conditions;

    // // 详细复杂考虑
    // static constexpr size_t num_of_time_samples = 8;
    // std::array<double, num_of_time_samples> time_samples;
    // for (size_t i = 1; i < num_of_time_samples; ++i)
    // {
    //     auto ratio =
    //         static_cast<double>(i) / static_cast<double>(num_of_time_samples - 1);
    //     time_samples[i] = constans.FLAGS_trajectory_time_length * ratio;
    // }
    // time_samples[0] = constans.FLAGS_polynomial_minimal_param;//0.01

    // vector<pair<vector<double>, double>> end_s_conditions;
    // for (const auto &time : time_samples)
    // {
    //     // 最大最小速度的获取
    //     double v_upper = std::min(feasibleregion.VUpper(time), ref_cruise_speed);
    //     double v_lower = feasibleregion.VLower(time);
    //     // st 上的规划
    //     vector<double> lower_end_s = { v_lower, 0.0};
    //     end_s_conditions.emplace_back(lower_end_s, time);

    //     vector<double>  upper_end_s = { v_upper, 0.0};
    //     end_s_conditions.emplace_back(upper_end_s, time);

    //     double v_range = v_upper - v_lower;
    //     // Number of sample velocities
    //     // 速度采样的个数
    //     size_t num_of_mid_points =
    //         std::min(static_cast<size_t>(constans.FLAGS_num_velocity_sample - 2),
    //                  static_cast<size_t>(v_range / constans.FLAGS_min_velocity_sample_gap));

    //     // 速度的分辨率，
    //     if (num_of_mid_points > 0)
    //     {
    //         double velocity_seg =
    //             v_range / static_cast<double>(num_of_mid_points + 1);
    //         for (size_t i = 1; i <= num_of_mid_points; ++i)
    //         {
    //             vector<double>  end_s = { v_lower + velocity_seg * static_cast<double>(i),0.0};
    //             end_s_conditions.emplace_back(end_s, time);
    //         }
    //     }
    // }
    // return end_s_conditions;
}

vector<pair<vector<double>, double>>
EndConditionSampler::SampleLonEndConditionsForStopping(const double ref_stop_point) const
{
    static constexpr size_t num_of_time_samples = 9;
    std::array<double, num_of_time_samples> time_samples;
    for (size_t i = 1; i < num_of_time_samples; ++i)
    {
        auto ratio =
            static_cast<double>(i) / static_cast<double>(num_of_time_samples - 1);
        time_samples[i] = constans.FLAGS_trajectory_time_length * ratio;
    }
    time_samples[0] = constans.FLAGS_polynomial_minimal_param;

    vector<pair<vector<double>, double>> end_s_conditions;
    for (const auto &time : time_samples)
    {
        vector<double> end_s = {std::max(init_s_[0], ref_stop_point), 0.0, 0.0};
        end_s_conditions.emplace_back(end_s, time);
    }
    return end_s_conditions;
}

vector<pair<vector<double>, double>>
EndConditionSampler::SampleLonEndConditionsForLaneChange(const vector<STBoundaryPoint> &stboundarypoint, const double cruise_speed) const
{
    vector<pair<vector<double>, double>> end_s_conditions;
    vector<SamplePoint> sample_points = QueryPathTimeObstacleSamplePoints(stboundarypoint, constans.FLAGS_vehicle_config_length,
                                                                          constans.FLAGS_trajectory_sample_s_num, cruise_speed);
    cout << "sample_points.size(): "<<sample_points.size() << endl;
    for (const SamplePoint &sample_point : sample_points)
    {
        double s = sample_point.s;
        double v = sample_point.ref_v;
        double t = sample_point.t;
        if (s > feasibleregion.SUpper(t) || s < feasibleregion.SLower(t))
        {
            continue;
        }
        vector<double> end_state = {s, v, 0.0};
        end_s_conditions.emplace_back(end_state, t);
    }
    return end_s_conditions;
}

vector<SamplePoint>
QueryPathTimeObstacleSamplePoints(const vector<STBoundaryPoint> &stboundarypoint, double vehicle_length,
                                  double sample_s_num, const double cruise_speed)
{
    vector<SamplePoint> sample_points;
    int index = stboundarypoint.size() - 1;
    while (!stboundarypoint.empty() && index >= 0)
    {
        auto p = stboundarypoint[index];
        if (p.su - p.sl > 2 * vehicle_length)
        {
            double s_upper = p.su - vehicle_length;
            double s_lower = p.sl + vehicle_length;
            double s_gap = (s_upper - s_lower) / sample_s_num;

            double v_follw = (stboundarypoint[index].su - stboundarypoint[index - 1].su) / 0.01;
            double v_overtake = (stboundarypoint[index].sl - stboundarypoint[index - 1].sl) / 0.01;

            double v=0;//采样可能需要细化
            if (v_follw == 0 && v_overtake == 0)
            {
                v = cruise_speed;
            }
            else if(v_follw != 0 && v_overtake == 0)
            {
                v = min(v_follw, cruise_speed);
            }
            else if(v_follw == 0 && v_overtake != 0)
            {
                v = cruise_speed;
            }
            else
            {
                v=min(v_follw, cruise_speed);
            }
            while (s_lower < s_upper)
            {
                SamplePoint samplepoint;
                samplepoint.t = stboundarypoint[index].t;
                samplepoint.s = s_lower;
                samplepoint.ref_v = v;
                s_lower += s_gap;
                sample_points.emplace_back(samplepoint);
            }
            break;
        }
        else
        {
            index--;
        }
    }

    return sample_points;
}
