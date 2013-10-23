#ifndef _POLYGON_H
#define _POLYGON_H

#include "../Utils/utils.h"

typedef tuple<double, double> slope_and_fix;

class Polygon
{
public:
	static Polygon * builder() { return new Polygon(); };
    Polygon * addPoint(f3d point) { vertice.push_back(point); return this; };
    Polygon * close();
    bool isInside(f3d point);
    double closestDistanceTo(f3d point);
    f3d closestPointTo(f3d point);

private:
	Polygon() {};
    list<f3d> vertice;
    list<slope_and_fix> slopes;
};

#endif
