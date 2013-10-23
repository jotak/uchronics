// -----------------------------------------------------------------
// IDLE ACTION
// -----------------------------------------------------------------
#include "IdleAction.h"
#include "DiscussionAction.h"
#include "BuyingAction.h"
#include "SellingAction.h"
#include "EatingAction.h"

bool isFood(InventoryObject * obj) {
	return obj->isFood();
}

// -----------------------------------------------------------------
// Name : IdleAction
// -----------------------------------------------------------------
IdleAction::IdleAction(AI * ai) : AIAction(ai)
{
}

// -----------------------------------------------------------------
// Name : ~IdleAction
// -----------------------------------------------------------------
IdleAction::~IdleAction()
{
}

// -----------------------------------------------------------------
// Name : update
// -----------------------------------------------------------------
void IdleAction::update(double delta)
{
	// Is starving?
	if (m_pAI->isStarving()) {
		list<InventoryObject*> inventory = m_pAI->getInventory();
		list<InventoryObject*>::iterator it = find_if(inventory.begin(), inventory.end(), isFood);
		if (it != inventory.end()) {
			// Eat what you have!
			m_pAI->doAction(new EatingAction(m_pAI));
			return;
		}
	}

	vector<AI*> lstNeighbours;
	m_pAI->getSurroundingAIs(&lstNeighbours);
	if (!lstNeighbours.empty()) {
		// Shuffle list, as there's no priority between neighbours
		random_shuffle(lstNeighbours.begin(), lstNeighbours.end());

		// Is starving?
		if (m_pAI->isStarving()) {
			// Find someone who sells food
			for (AI * other : lstNeighbours) {
				if (SellingAction::is(other, INVENTORY_TYPE_FOOD)) {
					int quantity = 1 + rand() % 4;
					m_pAI->doAction(new BuyingAction(m_pAI, other, INVENTORY_TYPE_FOOD, quantity));
					return;
				}
			}
		}

		// Want to talk?
		if (rand() % 10 == 0) {
			JoS_Element& dlg = m_pAI->pickDialog();
			if (!dlg.isNull()) {
				startDiscussion(m_pAI, dlg, lstNeighbours);
				return;
			}
		}

		// Want to buy something around?
	}
}

// -----------------------------------------------------------------
// Name : startDiscussion
//	static
// -----------------------------------------------------------------
void IdleAction::startDiscussion(AI * ai, JoS_Element& dialog, vector<AI*> &lstNeighbours)
{
	DiscussionAction * discussionAction = new DiscussionAction(ai);
	Discussion * pDiscussion = discussionAction->initiate(dialog);
	ai->doAction(discussionAction);
	for (AI * other : lstNeighbours) {
		joinDiscussion(other, pDiscussion);
	}
}

// -----------------------------------------------------------------
// Name : joinDiscussion
//	static
// -----------------------------------------------------------------
void IdleAction::joinDiscussion(AI * other, Discussion * pDiscussion)
{
	// TODO: evaluate if current action is actually more important than talking
	DiscussionAction * discussionAction = new DiscussionAction(other, pDiscussion);
	pDiscussion->join(discussionAction);
	other->doAction(discussionAction);
}
