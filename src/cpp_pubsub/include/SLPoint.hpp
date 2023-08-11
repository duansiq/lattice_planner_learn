#ifndef _SLPOINT_
#define _SLPOINT_

typedef struct SLPoint
{
    double s;
    double ds;
    double dds;

    double l;
    double dl;
    double ddl;

} SLPoint;
#endif