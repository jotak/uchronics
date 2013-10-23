// -----------------------------------------------------------------
// DISCUSSION
// -----------------------------------------------------------------
#include "Discussion.h"
#include "Actions/DiscussionAction.h"

// -----------------------------------------------------------------
// Name : Discussion
// -----------------------------------------------------------------
Discussion::Discussion(JoS_Element& dialog, DiscussionAction * pInitiator)
{
	m_pCurrentSentence = &dialog;
	m_pParticipants.push_back(pInitiator);
}

// -----------------------------------------------------------------
// Name : ~Discussion
// -----------------------------------------------------------------
Discussion::~Discussion()
{
}

// -----------------------------------------------------------------
// Name : join
// -----------------------------------------------------------------
void Discussion::join(DiscussionAction * pParticipant)
{
	m_pParticipants.push_back(pParticipant);
}

// -----------------------------------------------------------------
// Name : notifyStoppedTalking
// -----------------------------------------------------------------
void Discussion::notifyStoppedTalking(DiscussionAction * pPreviousTalker)
{
	int nbPotentialTalkers = 0;
	for (DiscussionAction * pTalker : m_pParticipants) {
		if (pTalker == pPreviousTalker) {
			pTalker->prepareResponse(JoS_Null::JoSNull);
		} else if (pTalker->prepareResponse(*m_pCurrentSentence)) {
			nbPotentialTalkers++;
		}
	}
	if (nbPotentialTalkers > 0) {
		int rndNextTalker = rand() % nbPotentialTalkers;
		for (DiscussionAction * pTalker : m_pParticipants) {
			if (pTalker->canRespond()) {
				if (rndNextTalker == 0) {
					m_pCurrentSentence = &(pTalker->respond());
					break;
				}
				rndNextTalker--;
			}
		}
	} else {
		// Everybody left!
		for (list<DiscussionAction*>::iterator it = m_pParticipants.begin(); it != m_pParticipants.end(); ++it) {
			(*it)->leaveDiscussion();
		}
		_gc->garbageMe(this);
	}
}
