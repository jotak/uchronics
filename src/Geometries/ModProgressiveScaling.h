#ifndef _MOD_PROGRESSIVE_SCALING_H
#define _MOD_PROGRESSIVE_SCALING_H

#include "GeometryModifier.h"

enum ProgressiveScalingBehavior
{
    PSB_Stop = 0,
    PSB_ForthAndBack,
    PSB_Repeat
};

class ModProgressiveScaling : public GeometryModifier
{
public:
    ModProgressiveScaling(float fScaleMin, float fScaleMax, float fScaleInit, float fCoef, float fScaleCenterX, float fScaleCenterY, ProgressiveScalingBehavior behavior);
    virtual ~ModProgressiveScaling();

    virtual void doTransforms(Color * pColor);
    virtual void update(double delta);

protected:
    ProgressiveScalingBehavior m_Behavior;
    float m_fScaleMin, m_fScaleMax, m_fCurrentScale;
    float m_fCoef;
    float m_fScaleCenterX, m_fScaleCenterY;
};

#endif
