#ifndef _FISHING_ACTION_H
#define _FISHING_ACTION_H

#include "AIAction.h"
#include "../../World/WaterArea.h"
#include "../../Data/JSonUtil.h"

class FishingAction : public AIAction
{
public:
	FishingAction(AI*, const JoS_Element&);
    virtual ~FishingAction();

    virtual void update(double);
    virtual bool isFinished() { return AIAction::isFinished() || pFishingArea == NULL; };

private:
    void findSpot();

    WaterArea * pFishingArea;
    double m_fWait;
    double abilityModifier;
};

#endif
