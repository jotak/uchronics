#ifndef _BUYING_ACTION_H
#define _BUYING_ACTION_H

#include "AIAction.h"

class BuyingAction : public AIAction
{
public:
	BuyingAction(AI*, AI*, string, int);
    virtual ~BuyingAction();

    virtual void update(double);

private:
    AI * seller;
    string goodTypeNeeded;
    int quantityNeeded;
};

#endif
