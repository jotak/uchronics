#include "ModColorize.h"

// -----------------------------------------------------------------
// Name : ModColorize
// -----------------------------------------------------------------
ModColorize::ModColorize(const Color &color) : GeometryModifier(), m_Color(color)
{
}

// -----------------------------------------------------------------
// Name : ~ModColorize
// -----------------------------------------------------------------
ModColorize::~ModColorize()
{
}

// -----------------------------------------------------------------
// Name : doTransforms
// -----------------------------------------------------------------
void ModColorize::doTransforms(Color * pColor)
{
    pColor->blend(&m_Color);
}
