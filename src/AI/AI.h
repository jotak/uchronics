#ifndef _AI_H
#define _AI_H

#include "../World/Character.h"
#include "Actions/AIAction.h"
#include "Timetable.h"
#include <stack>

#define DECISION_DELAY			0.5f
#define TRAITS_FRIENDLY			"friendly"

typedef tr1::function<bool (AI*, PartitionableItem*)> FilterPredicate;

class SellingAction;

class AI : public Character
{
public:
	static AI * buildAI(string jsonFile);
    virtual ~AI();

    virtual string toString() const;
    virtual void update(double delta);
    virtual bool isAI() { return true; };

    bool isIdle();
    void getSurroundingAIs(vector<AI*>*);
    JoS_Element& pickDialog();
    JoS_Element& pickDialog(JoS_Element&);
    void doAction(AIAction*);
    AIAction * getCurrentAction() { return pActionsList.front(); };

private:
	AI(JoS_Element * json);

	bool checkTimetable();
    void getSurroundingObjects(list<PartitionableItem*>*, FilterPredicate filter);
    void checkInteractions();
    void interact(GameObject*);
    float computeObjectiveAttraction(Character * pOther);
    void cleanFinishedActions();
    bool checkConditions(JoS_Element&);

    JoS_Element * json;
    double fInteractTimer;
    list<AIAction*> pActionsList;
    JoS_Union * dialogs;
    Timetable * pTimetable;
    Task * pCurrentTask;
};

#endif
