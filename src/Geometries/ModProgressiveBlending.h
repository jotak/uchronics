#ifndef _MOD_PROGRESSIVE_BLENDING_H
#define _MOD_PROGRESSIVE_BLENDING_H

#include "GeometryModifier.h"

class ModProgressiveBlending : public GeometryModifier
{
public:
    ModProgressiveBlending(Color * startColor, Color * endColor, float fPeriod);
    virtual ~ModProgressiveBlending();

    virtual void doTransforms(Color * pColor);
    virtual void update(double delta);

protected:
    Color * m_StartColor;
    Color * m_EndColor;
    float m_fPeriod;
    float m_fTimer;
};

#endif
