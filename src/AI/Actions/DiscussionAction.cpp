// -----------------------------------------------------------------
// DISCUSSION ACTION
// -----------------------------------------------------------------
#include "DiscussionAction.h"

#define TALKING_SPEED		20.0f	// 20 chars/sec

// -----------------------------------------------------------------
// Name : DiscussionAction
// -----------------------------------------------------------------
DiscussionAction::DiscussionAction(AI * ai) : AIAction(ai)
{
	m_pDiscussion = NULL;
	m_pResponse = NULL;
	m_fSentenceTimer = 0;
}

// -----------------------------------------------------------------
// Name : DiscussionAction
// -----------------------------------------------------------------
DiscussionAction::DiscussionAction(AI * ai, Discussion * pDiscussion) : AIAction(ai)
{
	m_pDiscussion = pDiscussion;
	m_pResponse = NULL;
	m_fSentenceTimer = 0;
}

// -----------------------------------------------------------------
// Name : ~DiscussionAction
// -----------------------------------------------------------------
DiscussionAction::~DiscussionAction()
{
}

// -----------------------------------------------------------------
// Name : initiate
// -----------------------------------------------------------------
Discussion * DiscussionAction::initiate(JoS_Element& sentence)
{
	m_pDiscussion = new Discussion(sentence, this);
	string text = sentence["text"].toString();
	m_pAI->say(text);
	m_fSentenceTimer = text.length() / TALKING_SPEED;
	return m_pDiscussion;
}

// -----------------------------------------------------------------
// Name : update
// -----------------------------------------------------------------
void DiscussionAction::update(double delta)
{
	if (!isFinished() && isTalking()) {
		m_fSentenceTimer -= delta;
		if (!isTalking()) {
			m_pDiscussion->notifyStoppedTalking(this);
		}
	}
}

// -----------------------------------------------------------------
// Name : isFinished
// -----------------------------------------------------------------
bool DiscussionAction::isFinished()
{
	return AIAction::isFinished() || m_pDiscussion == NULL;
}

// -----------------------------------------------------------------
// Name : isTalking
// -----------------------------------------------------------------
bool DiscussionAction::isTalking()
{
	return m_fSentenceTimer > 0;
}

// -----------------------------------------------------------------
// Name : prepareResponse
// -----------------------------------------------------------------
bool DiscussionAction::prepareResponse(JoS_Element& lastSentence)
{
	JoS_Element& response = m_pAI->pickDialog(lastSentence["responses"]);
	m_pResponse = (response.isNull()) ? NULL : &response;
	return m_pResponse != NULL;
}

// -----------------------------------------------------------------
// Name : respond
// -----------------------------------------------------------------
JoS_Element& DiscussionAction::respond()
{
	string text = (*m_pResponse)["text"].toString();
	m_pAI->say(text);
	m_fSentenceTimer = text.length() / TALKING_SPEED;
	return *m_pResponse;
}

// -----------------------------------------------------------------
// Name : leaveDiscussion
// -----------------------------------------------------------------
void DiscussionAction::leaveDiscussion()
{
	m_pDiscussion = NULL;
}
