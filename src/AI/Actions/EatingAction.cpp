// -----------------------------------------------------------------
// EATING ACTION
// -----------------------------------------------------------------
#include "EatingAction.h"
#include "../../World/WorldTime.h"
#include "../AI.h"

#define EATING_DURATION			600.0f // 10 min

// -----------------------------------------------------------------
// Name : EatingAction
// -----------------------------------------------------------------
EatingAction::EatingAction(AI * ai) : AIAction(ai)
{
	m_fTimer = 0;
}

// -----------------------------------------------------------------
// Name : ~EatingAction
// -----------------------------------------------------------------
EatingAction::~EatingAction()
{
}

// -----------------------------------------------------------------
// Name : eat
// -----------------------------------------------------------------
void EatingAction::eat(FoodObject * food)
{
	m_pAI->restoreEnergy(food->getRestore());
	m_pAI->resetHungryState();
	m_pAI->getInventory().remove(food);
	m_pAI->say("Ahh, good " + food->getName());
}

// -----------------------------------------------------------------
// Name : findFood
// -----------------------------------------------------------------
FoodObject * EatingAction::findFood(int energyNeeded)
{
	// Find food in player's inventory
	// Best food is a food that doesn't restore more than what is needed
	// okFood can restore more
	FoodObject * okFood = NULL;
	for (InventoryObject * obj : m_pAI->getInventory()) {
		if (obj->isFood()) {
			FoodObject * food = (FoodObject*) obj;
			if (food->getRestore() <= energyNeeded) {
				return food;
			}
			if (okFood == NULL) {
				okFood = food;
			}
		}
	}
	if (okFood != NULL) {
		return okFood;
	} else {
		m_pAI->say("There's nothing to eat!");
		return NULL;
	}
}

// -----------------------------------------------------------------
// Name : update
// -----------------------------------------------------------------
void EatingAction::update(double delta)
{
	if (m_fTimer <= 0) {
		// Need to eat?
		int energyNeeded = m_pAI->getMaxEnergy() - m_pAI->getEnergy();
		if (energyNeeded > 0) {
			m_fTimer = EATING_DURATION / TIME_COMPRESSION;
			FoodObject * food = findFood(energyNeeded);
			if (food != NULL) {
				eat(food);
			} else {
				// Nothing more to eat
				stop();
			}
		} else {
			m_pAI->say("I'm not hungry anymore.");
			stop();
		}
	} else {
		m_fTimer -= delta;
	}
}
