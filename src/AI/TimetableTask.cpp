// -----------------------------------------------------------------
// TIME TABLE TASK
// -----------------------------------------------------------------
#include "TimetableTask.h"

// -----------------------------------------------------------------
// Name : TimetableTask
// -----------------------------------------------------------------
TimetableTask::TimetableTask(int index, double time, AI * pAI, JoS_Element * taskData) : Task(pAI, taskData)
{
	this->index = index;
	this->time = time;
}

// -----------------------------------------------------------------
// Name : ~TimetableTask
// -----------------------------------------------------------------
TimetableTask::~TimetableTask()
{
}
