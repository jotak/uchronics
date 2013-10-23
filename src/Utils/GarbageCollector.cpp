// -----------------------------------------------------------------
// GARBAGE COLLECTOR
// -----------------------------------------------------------------
#include "GarbageCollector.h"

GarbageCollector * GarbageCollector::m_pInstance = NULL;

// -----------------------------------------------------------------
// Name : GarbageCollector
// -----------------------------------------------------------------
GarbageCollector::GarbageCollector()
{
}

// -----------------------------------------------------------------
// Name : ~GarbageCollector
// -----------------------------------------------------------------
GarbageCollector::~GarbageCollector()
{
	FREEVEC(m_pGarbages);
}

// -----------------------------------------------------------------
// Name : run
// -----------------------------------------------------------------
void GarbageCollector::run()
{
	FREEVEC(m_pGarbages);
}
