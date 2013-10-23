#ifndef _ERRATIC_MOVE_ACTION_H
#define _ERRATIC_MOVE_ACTION_H

#include "AIAction.h"

class ErraticMoveAction : public AIAction
{
public:
	ErraticMoveAction(AI * ai);
    virtual ~ErraticMoveAction();

    virtual void update(double delta);

private:
    double m_fWait;
};

#endif
