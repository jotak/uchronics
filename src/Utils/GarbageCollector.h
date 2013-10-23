#ifndef _GARBAGE_COLLECTOR_H
#define _GARBAGE_COLLECTOR_H

#include "utils.h"

class Garbageable {};

class GarbageCollector
{
public:
    ~GarbageCollector();
    static GarbageCollector * getInstance()
    {
        if (m_pInstance == NULL) m_pInstance = new GarbageCollector();
        return m_pInstance;
    };

    void run();
    void garbageMe(Garbageable * p) { m_pGarbages.push_back(p); };

private:
    GarbageCollector();
    static GarbageCollector * m_pInstance;

    list<Garbageable*> m_pGarbages;
};

// Helper alias
#define _gc GarbageCollector::getInstance()

#endif
