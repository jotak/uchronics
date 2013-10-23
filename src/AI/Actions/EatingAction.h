#ifndef _EATING_ACTION_H
#define _EATING_ACTION_H

#include "AIAction.h"
#include "../../World/FoodObject.h"

class EatingAction : public AIAction
{
public:
	EatingAction(AI*);
    virtual ~EatingAction();

    virtual void update(double);

private:
    FoodObject * findFood(int);
    void eat(FoodObject*);

    double m_fTimer;
};

#endif
