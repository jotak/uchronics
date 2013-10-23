// -----------------------------------------------------------------
// TERRAIN
// -----------------------------------------------------------------
#include "Terrain.h"
#include "../Display/DisplayEngine.h"

// -----------------------------------------------------------------
// Name : Terrain
// -----------------------------------------------------------------
Terrain::Terrain()
{
}

// -----------------------------------------------------------------
// Name : ~Terrain
// -----------------------------------------------------------------
Terrain::~Terrain()
{
	FREEVEC(m_pGeometries);
	FREEVEC(m_WaterAreas);
}

// -----------------------------------------------------------------
// Name : setDimensions
// -----------------------------------------------------------------
void Terrain::setDimensions(double left, double right, double top, double bottom)
{
	m_TopLeft = f3d(left, top, BOARDPLANE);
	m_Dimensions = f3d(right, bottom, BOARDPLANE) - m_TopLeft;
}

// -----------------------------------------------------------------
// Name : display
// -----------------------------------------------------------------
void Terrain::display()
{
	for (Geometry* &pGeo : m_pGeometries) {
		pGeo->display(m_TopLeft, Color::white);
	}
}
