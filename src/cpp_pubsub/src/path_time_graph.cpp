#include "path_time_graph.hpp"

using namespace std;

void PathTimeGraph::Init(const vector<vector<double>> &dynamic_obstacle_sl, const vector<PathPoint> &referenceline,
                         const double s_start, const double s_end, const double t_start,
                         const double t_end, const array<double, 3> &init_d,const array<double, 3> &init_s)
{
    // 筛选自车前后最近的动态障碍车
    vector<double> nearest_front_vehicle;
    vector<double> nearest_back_vehicle;
    int nearest_front_index = -1;
    int nearst_back_index = -1;
    double nearst_front_distance = 200;
    double nearst_back_distance = 100;

    //查找自车待驶入车道上最近的车辆
    if (!dynamic_obstacle_sl.empty())
    {
        double vehicle_s = init_s[0];
        for (int i = 0; i < dynamic_obstacle_sl.size(); i++)
        {
            double obs_s = dynamic_obstacle_sl[i][0];
            double obs_l = dynamic_obstacle_sl[i][1];
            double obs_heading = dynamic_obstacle_sl[i][2];
            double obs_velocity = dynamic_obstacle_sl[i][3];
            cout << "vehicle_s: " << vehicle_s << endl;
            cout << "obs_s: " << obs_s << " obs_l:" << obs_l << " obs_heading:" << obs_heading
                 << " obs_velocity: " << obs_velocity << endl;
            // 判断所在车道在代驶入车道或者本车道
            if (abs(obs_l) > constans.FLAGS_road_width / 2. + constans.FLAGS_vehicle_config_width / 2. - 0.5)
                continue;
            // 查找最近的两辆车
            if (obs_s > vehicle_s)
            {
                if (obs_s - vehicle_s <= nearst_front_distance)
                {
                    nearst_front_distance = obs_s - vehicle_s;
                    nearest_front_index = i;
                }
            }
            if (obs_s < vehicle_s)
            {
                if (vehicle_s - obs_s <= nearst_back_distance)
                {
                    nearst_back_distance = vehicle_s - obs_s;
                    nearst_back_index = i;
                }
            }
        }
    }
    cout << "nearest_front_index: " << nearest_front_index << " nearst_back_index: " << nearst_back_index << endl;
    if (nearest_front_index != -1)
    {
        nearest_front_vehicle = dynamic_obstacle_sl[nearest_front_index];
    }
    if(nearst_back_index!=-1)
    {
        nearest_back_vehicle = dynamic_obstacle_sl[nearst_back_index];
    }

    // /*状态判断: 根据动态障碍物在参考线上的投影判断自车前后是否有车辆以及是否在待驶入车道上
    // LC_mode = 1、待驶入车道前方无车后方无车
    // LC_mode = 2、待驶入车道前方有车后方有车
    // LC_mode = 3、待驶入车道前方无车后方有车
    // LC_mode = 4、待驶入车道前方有车后方无车
    // */
    // if(nearest_front_index!=-1 && nearst_back_index!=-1)
    // {
    //     LCmode = 2;
    // }
    // else if(nearest_front_index!=-1 && nearst_back_index==-1)
    // {
    //     LCmode = 4;
    // }
    // else if(nearest_front_index==-1 && nearst_back_index==-1)
    // {
    //     LCmode = 1;
    // }
    // else
    // {
    //     LCmode = 3;
    // }


    //STboundary 初始化
    STboundary_.clear();
    double relative_time = t_start;
    while(relative_time<t_end)
    {
        STBoundaryPoint stpoint;
        stpoint.t = relative_time;
        stpoint.sl = s_start;
        stpoint.su = s_end;
        STboundary_.emplace_back(stpoint);
        relative_time += constans.FLAGS_trajectory_time_resolution;
    }
    
    //待驶入车道前方有车
    //更新上界
    if(!nearest_front_vehicle.empty())
    {
        double front_obs_s = nearest_front_vehicle[0];
        double front_obs_v = nearest_front_vehicle[3];
        for(auto& st:STboundary_)
        {
            double predicte_s = st.t * front_obs_v + front_obs_s;
            cout << "front_vehicle predicte_s:" << predicte_s << endl;
            if (predicte_s < st.su)
            {
                st.su = predicte_s;
            }
        }
    }

    //待驶入车道后方有车
    //更新下界
    if(!nearest_back_vehicle.empty())
    {
        double back_obs_s = nearest_back_vehicle[0];
        double back_obs_v = nearest_back_vehicle[3];
        for(auto& st:STboundary_)
        {
            double predicte_s = st.t * back_obs_v + back_obs_s;
            if(predicte_s> st.sl)
            {
                st.sl = predicte_s;
            }
        }
    }

}