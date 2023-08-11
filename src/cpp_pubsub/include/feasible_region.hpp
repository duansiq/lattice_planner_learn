#ifndef _FEASIBLE_REGION_
#define _FEASIBLE_REGION_
#include <array>
#include "constans.hpp"
#include <math.h>
#include <algorithm>
using namespace std;
class FeasibleRegion
{

public:
    FeasibleRegion(){}
    ~FeasibleRegion(){}
    void Init(const array<double, 3> &init_s);

    double SUpper(const double t) const;

    double SLower(const double t) const;

    double VUpper(const double t) const;

    double VLower(const double t) const;

    double TLower(const double s) const;

private:
    array<double, 3> init_s_;

    double t_at_zero_speed_;

    double s_at_zero_speed_;

    Constans constants;
};

#endif