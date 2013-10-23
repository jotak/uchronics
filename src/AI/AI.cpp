// -----------------------------------------------------------------
// AI
// -----------------------------------------------------------------
#include "AI.h"
#include "../Managers/WorldManager.h"
#include "../Managers/DebugManager.h"
#include "../Physics/SpacePart.h"
#include "Actions/IdleAction.h"
#include "Actions/SellingAction.h"
#include "Discussion.h"
#include "Utils/Dialogs.h"

// -----------------------------------------------------------------
// Name : AI
// -----------------------------------------------------------------
AI::AI(JoS_Element * json) : Character(*json)
{
	this->json = json;
	JoS_Element& jsonRef = *json;
	fInteractTimer = 0;
	pCurrentTask = NULL;

	// Dialogs
	dialogs = new JoS_Union(jsonRef["dialogs"]);
	// Aggregate template
	_dlg->aggregateTemplates(*dialogs, jsonRef["dialogsTpl"]);

	// Timetable
	JoS_Element& jsonTimetable = jsonRef["timetable"];
	if (jsonTimetable.isList()) {
		pTimetable = new Timetable(this, &jsonTimetable);
		// Initialize table by finding current task
		checkTimetable();
		if (pCurrentTask == NULL) {
			// No task => invalid time table => no time table
			_debug->error(getName() + " has invalid timetable");
			FREE(pTimetable);
		}
	} else {
		pTimetable = NULL;
	}
}

// -----------------------------------------------------------------
// Name : ~AI
// -----------------------------------------------------------------
AI::~AI()
{
	FREEVEC(pActionsList);
	FREE(json);
	FREE(dialogs);
	FREE(pTimetable);
}

// -----------------------------------------------------------------
// Name : toString
// -----------------------------------------------------------------
string AI::toString() const {
	std::ostringstream oss;
	oss << Character::toString();
	return oss.str();
}

// -----------------------------------------------------------------
// Name : buildAI
// -----------------------------------------------------------------
AI * AI::buildAI(string jsonFile)
{
	string sError;
	JoSon * json = JoSon::fromFile(string(AI_PATH) + jsonFile, &sError);
	if (json == NULL) {
		_debug->error(string("Error when loading json: ") + jsonFile + " - Error message: " + sError);
		return NULL;
	} else {
		return new AI(json);
	}
}

// -----------------------------------------------------------------
// Name : update
// -----------------------------------------------------------------
void AI::update(double delta)
{
	cleanFinishedActions();

	fInteractTimer -= delta;
	if (fInteractTimer <= 0) {
		// Time to take a decision!
		fInteractTimer = DECISION_DELAY;
		bool didSomething = false;
		if (pTimetable != NULL) {
			didSomething = checkTimetable();
		}
		if (!didSomething && pCurrentTask != NULL) {
			didSomething = pCurrentTask->checkThen();
		}
		if (!didSomething && pActionsList.empty()) {
			// Idle
			doAction(new IdleAction(this));
		}
	}

	if (!pActionsList.empty()) {
		// Update current action
		pActionsList.front()->update(delta);
	}

	Character::update(delta);
}

// -----------------------------------------------------------------
// Name : cleanFinishedActions
// -----------------------------------------------------------------
void AI::cleanFinishedActions()
{
	for (list<AIAction*>::iterator it = pActionsList.begin(); it != pActionsList.end(); ++it) {
		AIAction * action = *it;
		if (action->isFinished()) {
			if (pCurrentTask != NULL) {
				pCurrentTask->onActionFinished(action);
			}
			delete action;
			it = pActionsList.erase(it);
		}
	}
}

// -----------------------------------------------------------------
// Name : checkTimetable
//	Check AI's time table to see if we need to start a new task
// -----------------------------------------------------------------
bool AI::checkTimetable()
{
	Task * pPreviousTask = pCurrentTask;
	pCurrentTask = pTimetable->updateCurrentTask();
	return (pPreviousTask != pCurrentTask);
}

// -----------------------------------------------------------------
// Name : doAction
// -----------------------------------------------------------------
void AI::doAction(AIAction * pAction)
{
	pActionsList.push_front(pAction);
}

// -----------------------------------------------------------------
// Name : pickDialog
// -----------------------------------------------------------------
JoS_Element& AI::pickDialog()
{
	return pickDialog(*dialogs);
}

// -----------------------------------------------------------------
// Name : pickDialog
// -----------------------------------------------------------------
JoS_Element& AI::pickDialog(JoS_Element& listItems)
{
	list<int> lstAcceptableIdx;
	for (int itemIdx = 0; itemIdx < listItems.size(); itemIdx++) {
		const JoS_Element& item = listItems[itemIdx];
		if (checkConditions(item["condState"])) {
			// This item is acceptable
			lstAcceptableIdx.push_back(itemIdx);
		}
	}
	if (!lstAcceptableIdx.empty()) {
		int rnd = rand() % lstAcceptableIdx.size();
		list<int>::iterator it = lstAcceptableIdx.begin();
		for (; rnd > 0; it++, rnd--);
		return listItems[*it];
	}
	return JoS_Null::JoSNull;
}

// -----------------------------------------------------------------
// Name : checkConditions
// -----------------------------------------------------------------
bool AI::checkConditions(JoS_Element& listConditions)
{
	bool bMatchConditions = true;
	for (int j = 0; j < listConditions.size(); j++) {
		string condition = listConditions[j].toString();
		// "bang" will be xored with the state function
		// example:
		//	For condition "idle", match = bang XOR isIdle = 0 ^ isIdle = isIdle
		//	For condition "!idle", match = bang XOR isIdle = 1 ^ isIdle = !isIdle
		bool bang = false;
		if (condition[0] == '!') {
			bang = true;
			condition = condition.substr(1);
		}
		if (condition == DIALOG_CONDITION_IDLE) {
			bMatchConditions &= (bang^isIdle());
		} else if (condition == DIALOG_CONDITION_STARVING) {
			bMatchConditions &= (bang^isStarving());
		} else if (condition == DIALOG_CONDITION_HUNGRY) {
			bMatchConditions &= (bang^isHungry());
		} else if (condition == DIALOG_CONDITION_TIRED) {
			bMatchConditions &= (bang^isTired());
		} else if (!pActionsList.empty()) {
			// Do action-related condition check
			bool isActionCondition = (pActionsList.front()->getConditionName() == condition);
			bMatchConditions &= (bang^isActionCondition);
		}
	}
	return bMatchConditions;
}

/******************************************************************/
/*            PREDICATES FOR getSurroundingObjects                */
/******************************************************************/
bool isSurrounding(AI * that, PartitionableItem * pItem) {
	if (pItem != that) {
		// Skip "this" since the AI itself is in the list
		f3d vec = that->getPosition() - pItem->getPosition();
		if (vec.getSize() < AI_INTERACTION_RADIUS) {
			return true;
		}
	}
	return false;
}
bool isSurroundingCharacter(AI * that, PartitionableItem * pItem) {
	return ((GameObject*)pItem)->isCharacter() && isSurrounding(that, pItem);
}
bool isSurroundingAI(AI * that, PartitionableItem * pItem) {
	return ((GameObject*)pItem)->isAI() && isSurrounding(that, pItem);
}
/******************************************************************/
/*         END OF PREDICATES FOR getSurroundingObjects            */
/******************************************************************/

// --------------------------------------------------------------------
// Name : getSurroundingObjects
//	Initialize lstSurroundingObjects as empty list when calling method
// --------------------------------------------------------------------
void AI::getSurroundingObjects(list<PartitionableItem*> * lstReturn, FilterPredicate filter)
{
	list<PartitionableItem*> lstSurroundingObjects;
	list<PartitionableItem*> * lst = _world->getSpacePartition()->getDirectNeighbours(this);
	lstSurroundingObjects.insert(lstSurroundingObjects.end(), lst->begin(), lst->end());
	for (int i = 0; i < NB_INDIRECT_NEIGHBOURS_ZONES; i++) {
		lst = _world->getSpacePartition()->getIndirectNeighbours(this, i);
		if (lst != NULL) {
			lstSurroundingObjects.insert(lstSurroundingObjects.end(), lst->begin(), lst->end());
		}
	}

	// Apply filters
	for (PartitionableItem * pItem : lstSurroundingObjects) {
		if (filter(this, pItem)) {
			lstReturn->push_back(pItem);
		}
	}
}

// --------------------------------------------------------------------
// Name : getSurroundingAIs
//	Initialize lstSurroundingObjects as empty list when calling method
// --------------------------------------------------------------------
void AI::getSurroundingAIs(vector<AI*> * lstReturn)
{
	list<PartitionableItem*> lstSurrounding;
	getSurroundingObjects(&lstSurrounding, isSurroundingAI);
	transform(lstSurrounding.begin(), lstSurrounding.end(), back_inserter(*lstReturn), static_caster<PartitionableItem*, AI*>());
}

// -----------------------------------------------------------------
// Name : checkInteractions
// -----------------------------------------------------------------
void AI::checkInteractions()
{
	list<PartitionableItem*> lstSurrounding;
	getSurroundingObjects(&lstSurrounding, isSurrounding);

	for (PartitionableItem * pItem : lstSurrounding) {
		interact((GameObject*) pItem);
	}
}

// -----------------------------------------------------------------
// Name : interact
// -----------------------------------------------------------------
void AI::interact(GameObject * pOther)
{
	if (pOther->isCharacter()) {
		float f = computeObjectiveAttraction((Character*) pOther);
		cout << "interaction [" << *this << ", " << *(Character*) pOther << "] :" << f << endl;
	}
}

// -----------------------------------------------------------------
// Name : computeObjectiveAttraction
// -----------------------------------------------------------------
float AI::computeObjectiveAttraction(Character * pOther)
{
	float attraction = 0.0f;
	long_hash * pThisTraits = getTraits();
	long_hash * pOtherTraits = pOther->getTraits();
	for (pair<string, long> from : *pThisTraits) {
		for (pair<string, long> to : *pOtherTraits) {
			// fRelation [-1,1]
			float fRelation = (float)from.second/*[0,5]*/ * (float)to.second/*[0,5]*/ * getTraitsRelation(from.first, to.first)/*[0,1]*/ / (float)(TRAIT_MAX_VALUE * TRAIT_MAX_VALUE);
			// Square it so that an "extreme" relation counts for more than a neutral one
			attraction += (fRelation * abs(fRelation));
		}
	}

	// "Friendly" traits automatically improves attraction
	float friendly = ((float)(*pThisTraits)[TRAITS_FRIENDLY]) / (float)(TRAIT_MAX_VALUE);
	attraction += friendly / 2;

	friendly = ((float)(*pOtherTraits)[TRAITS_FRIENDLY]) / (float)(TRAIT_MAX_VALUE);
	attraction += friendly;

	// charismatic, charmer, cold

	return attraction;
}

// -----------------------------------------------------------------
// Name : isIdle
// -----------------------------------------------------------------
bool AI::isIdle() {
	return pActionsList.empty() || pActionsList.front()->getType() == ACTION_IDLE;
}
