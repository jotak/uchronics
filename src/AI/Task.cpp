// -----------------------------------------------------------------
// TASK
//	A task is a set of things to do defined in json file.
//	One task can trigger several actions...
// -----------------------------------------------------------------
#include "Task.h"
#include "AI.h"
#include "../Managers/DebugManager.h"
#include "../Data/JSonUtil.h"
#include "Actions/FishingAction.h"
#include "Actions/EatingAction.h"
#include "Actions/SellingAction.h"

#define INSTRUCTION_GOTO				"goTo"
#define INSTRUCTION_START_ACTIVITY		"startActivity"
#define INSTRUCTION_THEN				"then"

#define ACTIVITY_FISHING				"fishing"
#define ACTIVITY_EAT					"eat"
#define ACTIVITY_SELL					"sell"

// -----------------------------------------------------------------
// Name : Task
// -----------------------------------------------------------------
Task::Task(AI * pAI, JoS_Element * taskData)
{
	this->taskDataPtr = taskData;
	m_pAI = pAI;
	isGoingTo = false;
	currentSubtask = NULL;
}

// -----------------------------------------------------------------
// Name : ~Task
// -----------------------------------------------------------------
Task::~Task()
{
	FREE(currentSubtask);
}

// -----------------------------------------------------------------
// Name : start
// -----------------------------------------------------------------
void Task::start()
{
	JoS_Element& taskData = *taskDataPtr;
	if (!taskData[INSTRUCTION_GOTO].isNull()) {
		executeGoTo(taskData[INSTRUCTION_GOTO]);
	}
	if (!taskData[INSTRUCTION_START_ACTIVITY].isNull()) {
		executeStartActivity(taskData[INSTRUCTION_START_ACTIVITY]);
	}
}

// -----------------------------------------------------------------
// Name : stop
// -----------------------------------------------------------------
void Task::stop()
{
	// If currentSubtask is not NULL, then we are currently executing a subtask
	if (currentSubtask != NULL) {
		currentSubtask->stop();
	}

	for (AIAction * action : doingActions) {
		action->stop();
	}
}

// -----------------------------------------------------------------
// Name : checkThen
//	Task may look like:
//task: {
//	goTo: [6, 3.5],
//	then: {
//		startActivity: {
//			name: fishing,
//			ability: 3
//		}
//	}
//}
//
//	the "then" keyword refers to a subtask that will be executed once the current task is finished
// The succession of "task", "then #1", "then #2" etc. are chained (each node contains its child, "task" is the root node).
// -----------------------------------------------------------------
bool Task::checkThen()
{
	// If currentSubtask is not NULL, then we are currently executing a subtask
	if (currentSubtask != NULL) {
		return currentSubtask->checkThen();
	} else {
		// Not in a subtask => check if that task is finished
		// If true, start the subtask
		JoS_Element& taskData = *taskDataPtr;
		if (!taskData[INSTRUCTION_THEN].isNull()) {
			if (isGoingTo && m_pAI->isCloseTo(goingTo)) {
				isGoingTo = false;
			}
			if (doingActions.empty() && !isGoingTo) {
				currentSubtask = new Task(m_pAI, &(taskData[INSTRUCTION_THEN]));
				currentSubtask->start();
				return true;
			}
		}
		return NULL;
	}
}

// -----------------------------------------------------------------
// Name : executeGoTo
// -----------------------------------------------------------------
void Task::executeGoTo(const JoS_Element& json)
{
	if (json.isList()) {
		double x = JSonUtil::getDouble(json[0]);
		double y = JSonUtil::getDouble(json[1]);
		cout << m_pAI->getName() << " goes to (" << x << ", " << y << ")" << endl;
		m_pAI->goToGround(x, y);
	} else {
		_debug->error("Invalid json, goTo expects list.");
	}
}

// -----------------------------------------------------------------
// Name : onActionFinished
// -----------------------------------------------------------------
void Task::onActionFinished(AIAction * action)
{
	// We're not sure if the action was initiated by this task or not... anyway, remove it if we find it
	// If currentSubtask is not NULL, then we are currently executing a subtask
	if (currentSubtask != NULL) {
		currentSubtask->onActionFinished(action);
	}
	doingActions.remove(action);
}

// -----------------------------------------------------------------
// Name : addAction
// -----------------------------------------------------------------
void Task::addAction(AIAction * action)
{
	doingActions.push_back(action);
	m_pAI->doAction(action);
}

// -----------------------------------------------------------------
// Name : executeStartActivity
// -----------------------------------------------------------------
void Task::executeStartActivity(const JoS_Element& json)
{
	if (json.isMap()) {
		string name = json["name"].toString();
		cout << m_pAI->getName() << " starts activity " << name << endl;
		if (name == ACTIVITY_FISHING) {
			addAction(new FishingAction(m_pAI, json));
		}
		else if (name == ACTIVITY_EAT) {
			addAction(new EatingAction(m_pAI));
		}
		else if (name == ACTIVITY_SELL) {
			addAction(new SellingAction(m_pAI, json));
		}
	} else {
		_debug->error("Invalid json, startActivity expects map.");
	}
}
