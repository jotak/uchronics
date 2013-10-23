// -----------------------------------------------------------------
// Polygon
// -----------------------------------------------------------------
#include "Polygon.h"

#define INFINITE  999.0f

// -----------------------------------------------------------------
// Name : isInside
//	Tells wether "point" is inside the polygon, or not
//	Algorithm of Crossing Numbers is used here
// -----------------------------------------------------------------
bool Polygon::isInside(f3d point)
{
	int nbVertice = vertice.size();
	if (nbVertice < 3) {
		return false;
	}
	bool inside = false;
	f3d vertA = *(vertice.rbegin());
	list<slope_and_fix>::iterator itSlope = slopes.begin();
    for (f3d vertB : vertice) {
    	slope_and_fix slope = *itSlope;
    	if (((vertA.y >= point.y) != (vertB.y >= point.y)) &&
    			// if vertB.y == vertA.y then "slope" is "INFINITE"
    			(vertB.y == vertA.y || point.x <= point.y * get<0>(slope) + get<1>(slope))) {
    		inside = !inside;
    	}
    	vertA = vertB;
    	++itSlope;
    }
    return inside;
}

// -----------------------------------------------------------------
// Name : closestDistanceTo
//	Returns the closest distance between the polygon and the input point
// -----------------------------------------------------------------
double Polygon::closestDistanceTo(f3d point)
{
	double closestDistance = -1;
	f3d vertA = *(vertice.rbegin());
    for (f3d vertB : vertice) {
    	f3d closest = closestPoint(point, vertA, vertB, true/*segment*/);
    	double distance2 = (point-closest).getSize2();
    	if (closestDistance < 0 || distance2 < closestDistance) {
    		closestDistance = distance2;
    	}
    	vertA = vertB;
    }
    return sqrt(closestDistance);
}

// -----------------------------------------------------------------
// Name : closestPointTo
//	Returns the closest point of polygon from the input point
// -----------------------------------------------------------------
f3d Polygon::closestPointTo(f3d point)
{
	f3d bestPoint;
	double closestDistance = -1;
	f3d vertA = *(vertice.rbegin());
    for (f3d vertB : vertice) {
    	f3d closest = closestPoint(point, vertA, vertB, true/*segment*/);
    	double distance2 = (point-closest).getSize2();
    	if (closestDistance < 0 || distance2 < closestDistance) {
    		closestDistance = distance2;
    		bestPoint = closest;
    	}
    	vertA = vertB;
    }
    return bestPoint;
}

// -----------------------------------------------------------------
// Name : close
//	"closes" the polygon
//	Used to pre-compute some data
// -----------------------------------------------------------------
Polygon * Polygon::close()
{
	int nbVertice = vertice.size();
	// No need to compute slopes for less than 3 vertice
	if (nbVertice >= 3) {
		f3d vertA = *(vertice.rbegin());
		for (f3d vertB : vertice) {
			// "INFINITE" is not accurate but not actually used; just there to avoid division by 0
			double slope = (vertB.y == vertA.y) ? INFINITE : (vertB.x - vertA.x) / (vertB.y - vertA.y);
			double fix = -vertA.y * slope + vertA.x;
			slopes.push_back(slope_and_fix(slope, fix));
			vertA = vertB;
		}
	}
    return this;
}
