#ifndef _OBSTACLE_
#define _OBSTACLE_
#include <vector>
#include <iostream>
#include <fstream>
#include <string.h>
#include "path_matcher.hpp"
#include "SLPoint.hpp"
#include "cartesian_frenet_converter.hpp"
using namespace std;
class Obstacle
{

public:
    Obstacle(){};
    ~Obstacle(){};
    //接收车辆坐标系下的障碍物车辆坐标
    //接收全局坐标系下的障碍物车辆坐标
    //全局坐标系障碍物车辆载入，使用TXT载入数据
    void get_obs_from_txt();
    vector<vector<double>> get_obs_static() { return obs_static_gcs; };
    vector<vector<double>> get_obs_dynamic() { return obs_dynamic_gcs; };
    vector<vector<double>> get_obs_gcs() { return obs_gcs; };
    vector<vector<SLPoint>> get_obs_predicte_sl() { return obs_dynamic_predict_sl; };
    void obs_gcs_to_frenet();

private:
    vector<vector<double>> obs_gcs;
    vector<vector<double>> obs_static_gcs;
    vector<vector<double>> obs_dynamic_gcs;
    vector<vector<double>> obs_static_frenet;
    vector<vector<SLPoint>> obs_dynamic_predict_sl;
    Cartesian_Frenet_Converter cartesion2frenet;
};

#endif