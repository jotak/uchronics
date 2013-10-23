// -----------------------------------------------------------------
// BUYING ACTION
// -----------------------------------------------------------------
#include "BuyingAction.h"
#include "../AI.h"

// -----------------------------------------------------------------
// Name : BuyingAction
// -----------------------------------------------------------------
BuyingAction::BuyingAction(AI * ai, AI * seller, string goodTypeNeeded, int quantityNeeded) : AIAction(ai)
{
	this->seller = seller;
	this->goodTypeNeeded = goodTypeNeeded;
	this->quantityNeeded = quantityNeeded;
}

// -----------------------------------------------------------------
// Name : ~BuyingAction
// -----------------------------------------------------------------
BuyingAction::~BuyingAction()
{
}

// -----------------------------------------------------------------
// Name : update
// -----------------------------------------------------------------
void BuyingAction::update(double delta)
{
}
