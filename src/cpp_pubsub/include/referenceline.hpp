#ifndef _REFERENCE_LINE_
#define _REFERENCE_LINE_
#include <vector>
#include <pathpoint.hpp>
#include <iostream>
#include <fstream>
#include <string.h>
#include <math.h>
using namespace std;

class ReferenceLine
{
public:
    ReferenceLine();
    ~ReferenceLine();
    void cal_referenceline();
    void get_globalpath_from_txt();
    vector<PathPoint> get_referenceline() { return reference_line; };

private:
    vector<PathPoint> reference_line;
    vector<PathPoint> global_path;

};

#endif