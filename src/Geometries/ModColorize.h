#ifndef _MOD_COLORIZE_H
#define _MOD_COLORIZE_H

#include "GeometryModifier.h"

class ModColorize : public GeometryModifier
{
public:
    ModColorize(const Color &color);
    virtual ~ModColorize();

    virtual void doTransforms(Color * pColor);
    virtual void update(double delta) {};

protected:
    Color m_Color;
};

#endif
