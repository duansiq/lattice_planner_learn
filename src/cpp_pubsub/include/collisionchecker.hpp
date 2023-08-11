#ifndef _COLLISIONCHECKER_
#define _COLLISIONCHECKER_
#include<vector>
#include "latpath1d.hpp"
#include "lonspeed1d.hpp"
#include "constraint_checker1d.hpp"
#include <math.h>
#include "SLPoint.hpp"
using namespace std;
class Collision_Checker
{
public:
    Collision_Checker();
    ~Collision_Checker();

    bool InCollision(const vector<vector<double>> &obs_static_frenet, const LATpath &lat_trajectory,
                     const double obs_width, const double obs_length, const std::array<double, 3> &init_s);

private:
    Constraint_Checker1d constraint_checker;
    array<double, 3> init_s_;

};

#endif