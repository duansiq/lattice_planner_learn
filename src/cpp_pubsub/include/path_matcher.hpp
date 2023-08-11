#ifndef _PATH_MATCHER_
#define _PATH_MATCHER_
#include "pathpoint.hpp"
#include <vector>
#include <math.h>
#include <algorithm>
#include "liner_interpolation.hpp"
#include <iostream>
using namespace std;
class Path_Matcher
{
private:

    PathPoint FindProjectionPoint(const PathPoint &p0, const PathPoint &p1,
                                  const double x, const double y);
    Liner_Interpolation linerinterpolation;

public:
    // Path_Matcher() = delete;

    PathPoint MatchToPath(const std::vector<PathPoint> &reference_line,
                          const double x, const double y);

    std::pair<double, double> GetPathFrenetCoordinate(
        const std::vector<PathPoint> &reference_line, const double x,
        const double y);

    void MatchToPath(const std::vector<PathPoint> &reference_line,PathPoint &matched_ref_point,
                          const double s);
};

#endif