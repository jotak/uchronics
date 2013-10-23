#ifndef _SELLING_ACTION_H
#define _SELLING_ACTION_H

#include "AIAction.h"
#include "../../Data/JSonUtil.h"

#define COST_TYPE_VERY_CHEAP		1
#define COST_TYPE_CHEAP				2
#define COST_TYPE_AVERAGE			3
#define COST_TYPE_EXPENSIVE			4
#define COST_TYPE_VERY_EXPENSIVE	5

class MerchandiseType
{
public:
	MerchandiseType(string type, u8 costType) {
		this->type = type;
		this->costType = costType;
	};
	string type;
	u8 costType;
};

class SellingAction : public AIAction
{
public:
	SellingAction(AI*, const JoS_Element&);
    virtual ~SellingAction();

    virtual void update(double);
    int getType() { return ACTION_SELLING; };
    list<MerchandiseType>& getMerchandiseTypes() { return lstMerchandiseTypes; };

    // Yoda sematic: selling are you tomatoes?
    bool static is(AI*, string);

private:
    double abilityModifier;
    list<MerchandiseType> lstMerchandiseTypes;
};

#endif
