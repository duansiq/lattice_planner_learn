#ifndef _PATH_TIME_GRAPH_
#define _PATH_TIME_GRAPH_
#include "pathpoint.hpp"
#include <vector>
#include <array>
#include "constans.hpp"
#include "STBoundaryPoint.hpp"
#include "iostream"
#include <string.h>

using namespace std;



class PathTimeGraph
{

public:
    PathTimeGraph(){};
    ~PathTimeGraph(){};
    void Init(const vector<vector<double>> &dynamic_obstacle_sl, const vector<PathPoint> &referencelineconst,
              const double s_start, const double s_end, const double t_start,
              const double t_end, const array<double, 3> &init_d,const array<double, 3> &init_s);
    vector<STBoundaryPoint> get_STBoundary() { return STboundary_; };
    int get_LCmode() { return LCmode; };

private:
    vector<PathPoint> referenceline_points_;
    Constans constans;
    vector<STBoundaryPoint> STboundary_;
    int LCmode=-1;
};

#endif