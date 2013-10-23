// -----------------------------------------------------------------
// ERRATIC MOVE ACTION
// -----------------------------------------------------------------
#include "ErraticMoveAction.h"
#include "../../Physics/MovesHelper.h"
#include "../AI.h"

// -----------------------------------------------------------------
// Name : ErraticMoveAction
// -----------------------------------------------------------------
ErraticMoveAction::ErraticMoveAction(AI * ai) : AIAction(ai)
{
	m_fWait = 0;
}

// -----------------------------------------------------------------
// Name : ~ErraticMoveAction
// -----------------------------------------------------------------
ErraticMoveAction::~ErraticMoveAction()
{
}

// -----------------------------------------------------------------
// Name : update
// -----------------------------------------------------------------
void ErraticMoveAction::update(double delta)
{
	if (!m_pAI->hasMoveTarget()) {
		if (m_fWait <= 0) {
			m_fWait = (rand() % 2) * FRAND100(3);	// 0-3 seconds
			f3d pos = m_pAI->getPosition();
			m_pAI->goToGround(pos.x + 1 - FRAND100(2), pos.y + 1 - FRAND100(2));
		} else {
			m_fWait -= delta;
		}
	}
}
