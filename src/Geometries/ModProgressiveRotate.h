#ifndef _MOD_PROGRESSIVE_ROTATE_H
#define _MOD_PROGRESSIVE_ROTATE_H

#include "GeometryModifier.h"

class ModProgressiveRotate : public GeometryModifier
{
public:
    ModProgressiveRotate(float fSpeed, float fCenterX, float fCenterY);
    virtual ~ModProgressiveRotate();

    virtual void doTransforms(Color * pColor);
    virtual void update(double delta);

protected:
    float m_fAngle;
    float m_fSpeed; // degree per second
    float m_fRotateCenterX;
    float m_fRotateCenterY;
};

#endif
