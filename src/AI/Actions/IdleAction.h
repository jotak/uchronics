#ifndef _IDLE_ACTION_H
#define _IDLE_ACTION_H

#include "AIAction.h"
#include "../../Data/JSonUtil.h"

class Discussion;

class IdleAction : public AIAction
{
public:
	IdleAction(AI*);
    virtual ~IdleAction();
    virtual void update(double);

    int getType() { return ACTION_IDLE; };

private:
    static void startDiscussion(AI*, JoS_Element&, vector<AI*>&);
    static void joinDiscussion(AI*, Discussion*);
};

#endif
