#include "manager.hpp"
Manage_Planner::Manage_Planner() {}
Manage_Planner::~Manage_Planner() {}
// 参考线离散化
/*
    PathPoint (x y v a theta kappa dkappa s) 全局坐标 参考线数据格式，匹配点，局部路径
*/
    
vector<PathPoint> Manage_Planner::ToDiscretizedReferenceLine(const vector<PathPoint> &ref_points)
{
    double s = 0.0;
    std::vector<PathPoint> path_points;
    for (const auto &ref_point : ref_points)
    {
        PathPoint path_point;
        path_point = ref_point;

        if (!path_points.empty())
        {
            double dx = path_point.x - path_points.back().x;
            double dy = path_point.y - path_points.back().y;
            s += std::sqrt(dx * dx + dy * dy);
        }
        path_point.s = s;
        path_points.push_back(std::move(path_point));
    }
    return path_points;
}

// frenet坐标系初始化
void Manage_Planner::ComputeInitFrenetState(const PathPoint &matched_point,
                                            const TrajectoryPoint &cartesian_state,
                                            std::array<double, 3> *ptr_s,
                                            std::array<double, 3> *ptr_d)
{
    cartesianfrenetconverter.cartesian_to_frenet(matched_point.s, matched_point.x, matched_point.y,
                                                 matched_point.theta, matched_point.kappa, matched_point.dkappa,
                                                 cartesian_state.x, cartesian_state.y,
                                                 cartesian_state.v, cartesian_state.a,
                                                 cartesian_state.theta,
                                                 cartesian_state.kappa, ptr_s, ptr_d);
}

void Manage_Planner::ComputeObsFrenetState(const PathPoint &matched_point,
                                           const double x, const double y,
                                           double *ptr_s,
                                           double *ptr_d)
{
    cartesianfrenetconverter.cartesian_to_frenet(matched_point.s, matched_point.x, matched_point.y,
                                                 matched_point.theta, x, y, ptr_s, ptr_d);
}

// 主函数
/*
    //1、参考线获取
    //2、匹配点计算
    //3、初始化坐标系Frenet，计算起始点坐标
    //4、获取当前时刻
    //5、障碍车投影 轨迹预测
    //6、构造pathtimegraph
    //7、设置速度限制
    //8、规划目标点获取
    //9、横向纵向轨迹簇生成
    //10、一维轨迹簇的可用性筛选
    //11、轨迹对的的匹配并计算总cost
    //12、选取最小cost的轨迹对合并
    //13、轨迹的可用性判断 横纵向加速度 曲率
    //14、碰撞检测
*/
void Manage_Planner::planner()
{
    cout << "main planner " << endl;
    high_resolution_clock::time_point start_time = high_resolution_clock::now();
    high_resolution_clock::time_point current_time = start_time;

    // 1.1规划起点计算(使用自定义点替代)
    TrajectoryPoint planning_init_point;
    // planning_init_point.x = 5.4;
    // planning_init_point.y = 2.7;
    ifstream readstart;
    readstart.open("/root/planner_ws/src/cpp_pubsub/txt/planstartpoint.txt");
    readstart >> planning_init_point.x;
    readstart >> planning_init_point.y;
    readstart >> planning_init_point.v;
    readstart >> planning_init_point.a;
    readstart.close();
    // 1、参考线获取
    // 1.2参考线获取（txt)读取
    referenceline_points = ToDiscretizedReferenceLine(referenceline.get_referenceline());
    auto ptr_reference_line = make_shared<vector<PathPoint>>(referenceline_points);
    // for (int i = 0; i < 10; i++)
    // {
    //     cout << "referenceline point :" << referenceline_points[i].x << " " << referenceline_points[i].y << " " << referenceline_points[i].s << endl;
    // }


    // planning_init_point.x = 23;
    // planning_init_point.y = 2.7+1.0;

    // 2、匹配点计算
    PathPoint matched_point = pathmatcher.MatchToPath(
        *ptr_reference_line, planning_init_point.x,
        planning_init_point.y);
    // 3、初始化坐标系Frenet，计算起始点坐标
    array<double, 3> init_s{0, 0, 0};
    array<double, 3> init_d{0, 0, 0};
    ComputeInitFrenetState(matched_point, planning_init_point, &init_s, &init_d);
    cout << " matched point: x= " << matched_point.x << " y= " << matched_point.y << " s= " << matched_point.s << endl;
    cout << " init_s " << init_s[0] << " " << init_s[1] << " " << init_s[2] << endl;
    cout << " init_d " << init_d[0] << " " << init_d[1] << " " << init_d[2] << endl;
    // 4、获取当前时刻
    current_time = high_resolution_clock::now();

    // 5、障碍车投影
    static vector<vector<double>> obs_gcs;
    obstacle.get_obs_from_txt();
    obs_gcs = obstacle.get_obs_gcs();
    cout << "obs_gcs size: " << obs_gcs.size() << endl;



    // 5.1障碍物投影到frenet坐标系
    vector<vector<double>> obs_static_frenet;
    vector<vector<double>> obs_dynamic_frenet;
    if (!obs_gcs.empty())
    {
        for (auto const obs : obs_gcs)
        {
            double x = obs[0];
            double y = obs[1];
            double obs_heading = obs[2];
            double obs_velocity = obs[3];
            PathPoint matchpoint = pathmatcher.MatchToPath(
                *ptr_reference_line, x, y);
            // cout << "matchpoint :  " << matchpoint.x << " " << matchpoint.y << " " << matchpoint.s << endl;
            double obs_s = 0.;
            double obs_d = 0.;
            ComputeObsFrenetState(matchpoint, x, y, &obs_s, &obs_d);
            // cout << " obs point : " << obs_s << " " << obs_d << endl;
            vector<double> p;
            p.emplace_back(obs_s);
            p.emplace_back(obs_d);
            p.emplace_back(obs_heading);
            p.emplace_back(obs_velocity);
            if(obs_velocity<=0.5)
            {
                obs_static_frenet.emplace_back(p);
            }
            else
            {
                obs_dynamic_frenet.emplace_back(p);
            }
        }
        // cout << " obs cartesion: x= " << obs_static_gcs[0][0] << " y= " << obs_static_gcs[0][1] << endl;
        // cout << " obs frenet: s= " << obs_static_frenet[0][0] << " d= " << obs_static_frenet[0][1] << endl;
    }
    cout << "obs_static_frenet size: " << obs_static_frenet.size() << endl;
    cout << "obs_dynamic_frenet size: " << obs_dynamic_frenet.size() << endl;


    // 7、设置速度限制
    double speed_limit = 30; // limit:10m/s

    // 8、规划目标点获取,应该是一些规划目标的信息，终点，巡航速度等，暂不设置

    // 9、横向纵向轨迹簇生成
    vector<LATpath> lat_trajectory1d_bundle;
    endconditionsampler.Init(init_s, init_d);
    // 9.1横向轨迹簇生成
    // 横向规划终点采样
    vector<pair<vector<double>, double>> lat_end_conditions;
    lat_end_conditions = endconditionsampler.SampleLatEndConditions();
    // 横向规划轨迹簇
    trajectory1dgenerator.GenerateLateralTrajectoryBundle(init_s, init_d, lat_end_conditions, lat_trajectory1d_bundle);
    // for (auto p : lat_trajectory1d_bundle)
    // {
    //     cout << p.a0 << ", " << p.a1 << ", " << p.a2 << ", " << p.a3 << " ," << p.a4 << " ," << p.a5 << " ," << p.smin << " ," << p.smax << endl;
    // }

    // 9.2纵向速度规划
    vector<LONspeed> lon_trajectory1d_bundle;
    vector<pair<vector<double>, double>> lon_end_conditions;
    vector<double> end_ds_candidates;
    vector<double> end_t_candidates;
    // int plan_mode = 0; // 巡航
    int plan_mode = 1; //变道
    // int plan_mode = 2; //停车

    // 规划末点采样
    double s_start = referenceline_points[0].s;
    double s_end = referenceline_points.back().s;
    double t_start = 0.;
    double t_end = constans.FLAGS_trajectory_time_length;
    pathtimegraph.Init(obs_dynamic_frenet, referenceline_points, s_start, s_end, t_start, t_end, init_d , init_s);
    vector<STBoundaryPoint> STboundarypoint=pathtimegraph.get_STBoundary();
    if(!STboundarypoint.empty())
    {
        for(auto p:STboundarypoint)
        {
            cout << "t: " << p.t << " sl: " << p.sl << " su: " << p.su << endl;
        }
    }
    double v_upper; // 道路限速
    if (plan_mode == 0)
    {
        //*巡航速度规划末点采样
        v_upper = speed_limit; // 道路限速
        lon_end_conditions = endconditionsampler.SampleLonEndConditionsForCruising(v_upper);
        trajectory1dgenerator.GenerateSpeedProfilesForCruising(init_s, lon_end_conditions, lon_trajectory1d_bundle);
    }
    else if (plan_mode == 1)
    {
        //*变道速度规划末点采样
        lon_end_conditions = endconditionsampler.SampleLonEndConditionsForLaneChange(STboundarypoint ,v_upper);
        for(auto condition:lon_end_conditions)
        {
            cout << "lon_end_conditions: s: " << condition.first[0] << " v: " 
            << condition.first[1] << " a: " << condition.first[0] << " t: " << condition.second << endl;
        }
    }
    else
    {
        //*停车速度规划末点采样
        double stop_point_s=10;
        lon_end_conditions = endconditionsampler.SampleLonEndConditionsForStopping(stop_point_s);
    }

    //*巡航速度规划
    //*前方参考线距离内无车
    //*前方参考线距离内有车无车判断
    //*判断障碍物在参考线的投影上
    //*变道速度规划

    //*停车速度规划
    // if (!lon_trajectory1d_bundle.empty())
    // {
    //     cout << " speed plan size: " << lon_trajectory1d_bundle.size() << endl;
    //     cout << " speed plan size: " << lon_trajectory1d_bundle.size() << endl;
    //     cout << " speed plan size: " << lon_trajectory1d_bundle.size() << endl;

    //     for (auto lon_tra : lon_trajectory1d_bundle)
    //     {
    //         cout << lon_tra.a0 << " " << lon_tra.a1 << " " << lon_tra.a2 << " " << lon_tra.a3 << " " << lon_tra.a4 << " " << lon_tra.tmin << " " << lon_tra.tmax << endl;
    //     }
    // }

    // 10、一维轨迹簇的可用性筛选
    trajectory_evaluator.TrajectoryEvaluator(init_s, lon_trajectory1d_bundle, lat_trajectory1d_bundle, referenceline_points);
    cout << " before collision path number: " << trajectory_evaluator.num_of_trajectory() << endl;

    // 14、碰撞检测
    bool latplanok = false;
    while (trajectory_evaluator.has_more_trajectory())
    {
        auto cost = trajectory_evaluator.gettop_cost();
        auto lat_trajectory = trajectory_evaluator.gettop_trajectory();
        if (collision_checker.InCollision(obs_static_frenet, lat_trajectory, 2, 5, init_s))
        {
            cout << " top cost: " << cost << endl;
            latplanok = true;
            latpath = lat_trajectory;
            break;
        }
        else
            continue;
    }

    cout << " after collision path number: " << trajectory_evaluator.num_of_trajectory() << endl;
    cout << "local path: " << latpath.a0 << " " << latpath.a1 << " " << latpath.a2 << " "
         << latpath.a3 << " " << latpath.a4 << " " << latpath.a5 << " " << latpath.smin << " "
         << latpath.smax << " " << latpath.end_s << " " << latpath.end_l << endl;

    // while (trajectory_evaluator.has_more_trajectory())
    // {
    //     auto cost = trajectory_evaluator.gettop_cost();
    //     auto lat_trajectory = trajectory_evaluator.gettop_trajectory();
    //     cout << " cost : " << cost << " lat_trajectory " << lat_trajectory.end_s << " " << lat_trajectory.end_l << endl;
    // }

    // 坐标系转化为笛卡尔坐标系
    if (latplanok)
    {
        // 五次多项式系数转化计算，计算为等间隔的点 slpoint
        latpath_gcs.clear();
        latpointcal.set_lat_coef(latpath);
        // latpointcal.set_lat_coef(testlatpath);

        double evaluation_horizon = latpath.smax;
        double FLAGS_trajectory_space_resolution = 0.5; // 间隔设置为0.5m
        for (double s = latpath.smin; s < evaluation_horizon; s += FLAGS_trajectory_space_resolution)
        {
            array<double, 3> s_condition;
            array<double, 3> d_condition;
            d_condition[0] = latpointcal.LATEvaluate(0, s - latpath.smin);
            d_condition[1] = latpointcal.LATEvaluate(1, s - latpath.smin);
            d_condition[2] = latpointcal.LATEvaluate(2, s - latpath.smin);
            s_condition[0] = s;
            s_condition[1] = 2; // 期望速度设置
            s_condition[2] = 0; // 期望加速度设置
            // 匹配点计算
            PathPoint matched_ref_point;
            pathmatcher.MatchToPath(*ptr_reference_line, matched_ref_point, s);
            // cout << " s=" << s << " l=" << d_condition[0] << " x=" << matched_ref_point.x << " y=" << matched_ref_point.y << " s= " << matched_ref_point.s << endl;
            double x = 0.0;
            double y = 0.0;
            double theta = 0.0;
            double kappa = 0.0;
            double v = 0.0;
            double a = 0.0;

            const double rs = matched_ref_point.s;
            const double rx = matched_ref_point.x;
            const double ry = matched_ref_point.y;
            const double rtheta = matched_ref_point.theta;
            const double rkappa = matched_ref_point.kappa;
            const double rdkappa = matched_ref_point.dkappa;
            cartesianfrenetconverter.frenet_to_cartesian(rs, rx, ry, rtheta, rkappa, rdkappa, s_condition, d_condition,
                                                         &x, &y, &theta, &kappa, &v, &a);
            TrajectoryPoint latpoint;
            latpoint.x = x;
            latpoint.y = y;
            latpoint.theta = theta;
            latpoint.kappa = kappa;
            latpoint.v = v;
            latpoint.a = a;
            latpoint.s = s;
            latpath_gcs.emplace_back(latpoint);
        }
        // 坐标转化
    }
}

// void Manage_Planner::planner()
// {
//     test_.test_include();
//     cout << "manager_planner: " << endl;
// }

void Manage_Planner::writetxt()
{
    ofstream write_referenceline("/root/planner_ws/src/cpp_pubsub/txt/referenceline_.txt", ios::trunc);

    for (auto point : referenceline_points)
    {
        write_referenceline << " x: " << point.x << "   ";
        write_referenceline << " y: " << point.y << "   ";
        write_referenceline << " theta: " << point.theta << "   ";
        write_referenceline << " kappa: " << point.kappa << "   ";
        write_referenceline << " dkappa: " << point.dkappa << "   ";
        write_referenceline << " s: " << point.s << endl;
    }
}
