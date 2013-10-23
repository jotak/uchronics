// -----------------------------------------------------------------
// FISHING ACTION
// -----------------------------------------------------------------
#include "FishingAction.h"
#include "../AI.h"
#include "../../Managers/WorldManager.h"
#include "../../World/WorldTime.h"
#include "../../World/Terrain.h"
#include "../../World/FinalObjects/Trout.h"
#include "../../World/FinalObjects/Carp.h"

#define MIN_WAIT_TIME			1800 // 30 min
#define MAX_WAIT_TIME			7200 // 120 min
#define FISHING_MIN_ABILITY		1
#define FISHING_MAX_ABILITY		4

// -----------------------------------------------------------------
// Name : FishingAction
// -----------------------------------------------------------------
FishingAction::FishingAction(AI * ai, const JoS_Element& json) : AIAction(ai)
{
	// Find nearest water point
	const list<WaterArea*>& areas = _world->getTerrain()->getWaterAreas();
	pFishingArea = NULL;
	double nearestDistance = -1;
	for (WaterArea * pArea : areas) {
		double distance = pArea->getArea()->closestDistanceTo(m_pAI->getPosition());
		if (nearestDistance == -1 || nearestDistance > distance) {
			pFishingArea = pArea;
			nearestDistance = distance;
		}
	}
	int ability = JSonUtil::getCappedInt(json["ability"], FISHING_MIN_ABILITY, FISHING_MAX_ABILITY, FISHING_MIN_ABILITY);
	abilityModifier = (double) ability / 2.0f; // from 0.5 to 2
	m_fWait = 0;
}

// -----------------------------------------------------------------
// Name : ~FishingAction
// -----------------------------------------------------------------
FishingAction::~FishingAction()
{
}

// -----------------------------------------------------------------
// Name : update
// -----------------------------------------------------------------
void FishingAction::update(double delta)
{
	if (m_fWait < 0) {
		findSpot();
	} else if (pFishingArea != NULL && !m_pAI->hasMoveTarget()) {
		m_fWait -= delta;
		// Does it bit?
		double treshold = delta * 60.0f * abilityModifier * pFishingArea->getFishingProbability();
		if (100.0f * rand() / RAND_MAX < treshold) {
			// Yes!
			FoodObject * pFish = NULL;
			int rnd = (int) FRAND100(100);
			if (rnd < pFishingArea->getTroutsPct()) {
				pFish = new Trout();
			} else {
				pFish = new Carp();
			}
			m_pAI->addToInventory(pFish);
		}
	}
}

// -----------------------------------------------------------------
// Name : findSpot
// -----------------------------------------------------------------
void FishingAction::findSpot()
{
	m_fWait = MIN_WAIT_TIME + rand() % (MAX_WAIT_TIME - MIN_WAIT_TIME);
	m_fWait /= TIME_COMPRESSION;
	if (pFishingArea == NULL) {
		m_pAI->say("There's no water around!");
	} else {
		// Move a bit around
		f3d rndPoint = m_pAI->getPosition() + f3d(FRAND100(3) - 1, FRAND100(3) - 1, 0);
		m_pAI->goToGround(pFishingArea->getArea()->closestPointTo(rndPoint));
	}
}
