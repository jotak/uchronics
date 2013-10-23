#ifndef _AI_ACTION_H
#define _AI_ACTION_H

#include "../../Utils/utils.h"

#define ACTION_UNKNOWN		0
#define ACTION_IDLE			1
#define ACTION_SELLING		2

class AI;

class AIAction
{
public:
	AIAction(AI * ai) { m_pAI = ai; m_bForceStop = false; };
    virtual ~AIAction() {};
    virtual void update(double delta) = 0;
    virtual bool isFinished() { return m_bForceStop; };
    virtual void stop() { m_bForceStop = true; };
    virtual int getType() { return ACTION_UNKNOWN; };
    virtual string getConditionName() { return ""; };

protected:
    AI * m_pAI;
    bool m_bForceStop;
};

#endif
