#ifndef _PATHPOINT_
#define _PATHPOINT_
typedef struct PathPoint
{
    double x;
    double y;
    double v;
    double a;
    double theta;
    double kappa;
    double dkappa;
    double ddkappa;
    double s;
    double t;
}PathPoint;
#endif