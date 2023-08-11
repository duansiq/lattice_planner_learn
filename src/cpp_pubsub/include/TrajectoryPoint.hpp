#ifndef _TRAJECTORY_POINT_
#define _TRAJECTORY_POINT_
typedef struct TrajectoryPoint
{
    double x=0;
    double y=0;
    double v=0;
    double a=0;
    double theta=0;
    double kappa=0;
    double dkappa=0;
    double ddkappa=0;
    double s=0;
    double t=0;
}TrajectoryPoint;

#endif