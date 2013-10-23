#ifndef _WATER_AREA_H
#define _WATER_AREA_H

#include "../Physics/Polygon.h"

class WaterArea
{
public:
	WaterArea(Polygon * area, double fishingProbability, int troutsPct, int carpsPct) {
		this->area = area;
		this->fishingProbability = fishingProbability;
		this->troutsPct = troutsPct;
		this->carpsPct = carpsPct;
	};
    ~WaterArea() {};

    Polygon * getArea() { return area; };
    double getFishingProbability() { return fishingProbability; };
    int getTroutsPct() { return troutsPct; };
    int getCarpsPct() { return carpsPct; };

private:
    Polygon * area;
    double fishingProbability;
    int troutsPct;
    int carpsPct;
};

#endif
