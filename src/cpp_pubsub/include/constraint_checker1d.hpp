#ifndef _CONSTRAINTCHECKER_
#define _CONSTRAINTCHECKER_
#include <array>
#include <vector>
#include "latpath1d.hpp"
#include "lonspeed1d.hpp"
#include "constans.hpp"

using namespace std;

class Constraint_Checker1d
{
public:
    Constraint_Checker1d();
    ~Constraint_Checker1d();
    bool IsValidLongitudinalTrajectory(const LONspeed &lon_trajectory);
    bool IsValidLateralTrajectory(const LATpath &lat_trajectory,
                                  const LONspeed &lon_trajectory);
    double LONEvaluate(const std::uint32_t order,
                       const double param) const;

    double LATEvaluate(const uint32_t order,
                       const double p) const;
    void set_lon_coef(const LONspeed &lon_trajectory);
    void set_lat_coef(const LATpath &lat_trajectory);

private:
    Constans constants;
    LONspeed lon_coef_;
    LATpath lat_coef_;
};

#endif