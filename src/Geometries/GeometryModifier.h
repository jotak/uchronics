#ifndef _GEOMETRY_MODIFIER_H
#define _GEOMETRY_MODIFIER_H

#include "../Utils/utils.h"

class GeometryModifier
{
public:
    GeometryModifier();
    virtual ~GeometryModifier();

    virtual void doTransforms(Color * pColor) {};
    virtual void update(double delta) {};
    bool isActive() { return m_bActive; };
    void setActive(bool bActive) { m_bActive = bActive; };
    bool isRunning() { return m_bRunning; };
    void setRunning(bool bRunning) { m_bRunning = bRunning; };

protected:
    bool m_bActive;   // Not active = not displayed and not updated
    bool m_bRunning;  // Not running = not updated
};

#endif
