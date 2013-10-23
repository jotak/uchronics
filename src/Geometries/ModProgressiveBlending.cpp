#include "ModProgressiveBlending.h"

// -----------------------------------------------------------------
// Name : ModProgressiveBlending
//  Constructor
// -----------------------------------------------------------------
ModProgressiveBlending::ModProgressiveBlending(Color * startColor, Color * endColor, float fPeriod) : GeometryModifier()
{
    m_StartColor = startColor;
    m_EndColor = endColor;
    m_fPeriod = fPeriod;
    m_fTimer = 0;
}

// -----------------------------------------------------------------
// Name : ~ModProgressiveBlending
//  Destructor
// -----------------------------------------------------------------
ModProgressiveBlending::~ModProgressiveBlending()
{
}

// -----------------------------------------------------------------
// Name : doTransforms
// -----------------------------------------------------------------
void ModProgressiveBlending::doTransforms(Color * pColor)
{
    Color blend(1, 1, 1, 1);
    float fHalfPeriod = m_fPeriod / 2;
    float mod = (m_fTimer / fHalfPeriod); // [0, 2]
    if (mod < 1) { // from start color to end color
        mod = 1 - mod;
    } else {  // from end color to start color
        mod = mod - 1;
    }
    blend = ((*m_StartColor) * mod + (*m_EndColor) * (1.0f-mod)) * 0.5f;
    *pColor = (*pColor) * blend;
}

// -----------------------------------------------------------------
// Name : update
// -----------------------------------------------------------------
void ModProgressiveBlending::update(double delta)
{
    m_fTimer += (float) delta;
    if (m_fTimer >= m_fPeriod) {
        m_fTimer = 0;
    }
}
