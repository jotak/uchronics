#ifndef _DISCUSSION_ACTION_H
#define _DISCUSSION_ACTION_H

#include "AIAction.h"
#include "../Discussion.h"

class DiscussionAction : public AIAction
{
public:
	DiscussionAction(AI * ai);
	DiscussionAction(AI * ai, Discussion * pDiscussion);
    ~DiscussionAction();

    Discussion * initiate(JoS_Element&);
    void update(double delta);
    bool isFinished();
    bool prepareResponse(JoS_Element&);
    bool canRespond() { return m_pResponse != NULL; };
    JoS_Element& respond();
    bool isTalking();
    void leaveDiscussion();

private:
    Discussion * m_pDiscussion;
    double m_fSentenceTimer;
    JoS_Element * m_pResponse;
};

#endif
