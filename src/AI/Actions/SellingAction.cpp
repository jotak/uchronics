// -----------------------------------------------------------------
// SELLING ACTION
// -----------------------------------------------------------------
#include "SellingAction.h"
#include "../AI.h"

#define SELLING_MIN_ABILITY		1
#define SELLING_MAX_ABILITY		4

// -----------------------------------------------------------------
// Name : SellingAction
// -----------------------------------------------------------------
SellingAction::SellingAction(AI * ai, const JoS_Element& json) : AIAction(ai)
{
	// Ability
	int ability = JSonUtil::getCappedInt(json["ability"], SELLING_MIN_ABILITY, SELLING_MAX_ABILITY, SELLING_MIN_ABILITY);
	abilityModifier = (double) ability / 2.0f; // from 0.5 to 2

	// Goods
	const JoS_Element& goods = json["goods"];
	int nbGoods = goods.size();
	for (int i = 0; i < nbGoods; i++) {
		const JoS_Element& good = goods[i];
		string cost = good["cost"].toString();
		u8 uCostType = COST_TYPE_VERY_CHEAP;
		if (cost == "cheap") {
			uCostType = COST_TYPE_CHEAP;
		} else if (cost == "average") {
			uCostType = COST_TYPE_AVERAGE;
		} else if (cost == "expensive") {
			uCostType = COST_TYPE_EXPENSIVE;
		} else if (cost == "very expensive") {
			uCostType = COST_TYPE_VERY_EXPENSIVE;
		}
		lstMerchandiseTypes.push_back(MerchandiseType(good["type"].toString(), uCostType));
	}
}

// -----------------------------------------------------------------
// Name : ~SellingAction
// -----------------------------------------------------------------
SellingAction::~SellingAction()
{
}

// -----------------------------------------------------------------
// Name : update
// -----------------------------------------------------------------
void SellingAction::update(double delta)
{
}

// -----------------------------------------------------------------
// Name : isSelling
//	static
// -----------------------------------------------------------------
bool SellingAction::is(AI * ai, string type) {
	AIAction * action = ai->getCurrentAction();
	if (action != NULL && action->getType() == ACTION_SELLING) {
		SellingAction * sellingAction = (SellingAction*) action;
		for (MerchandiseType merchandiseType : sellingAction->getMerchandiseTypes()) {
			if (merchandiseType.type == type) {
				return true;
			}
		}
	}
	return false;
}
