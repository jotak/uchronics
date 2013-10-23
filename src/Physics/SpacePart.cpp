// -----------------------------------------------------------------
// SPACE PARTITION
// -----------------------------------------------------------------
#include "SpacePart.h"

// -----------------------------------------------------------------
// Name : SpacePart
// -----------------------------------------------------------------
SpacePart::SpacePart(f3d topLeft, f3d dimensions, double fInteractionRadius)
{
	m_fInteractionRadius = fInteractionRadius;
	m_TopLeft = topLeft;
	m_Dimensions = dimensions;

	double d = m_Dimensions.x / m_fInteractionRadius;
	if (d == (double) (int) d) {
		m_iNbTilesAbs = (int) d;
	} else {
		m_iNbTilesAbs = 1 + (int) d;
		m_Dimensions.x = m_fInteractionRadius * m_iNbTilesAbs;
	}
	d = m_Dimensions.y / m_fInteractionRadius;
	if (d == (double) (int) d) {
		m_iNbTilesOrd = (int) d;
	} else {
		m_iNbTilesOrd = 1 + (int) d;
		m_Dimensions.y = m_fInteractionRadius * m_iNbTilesOrd;
	}

	// Tile size is calculated using interaction radius
	m_arrPartitions = new Partition[m_iNbTilesAbs * m_iNbTilesOrd];
	for (int y = 0; y < m_iNbTilesOrd; y++) {
		for (int x = 0; x < m_iNbTilesAbs; x++) {
			m_arrPartitions[x + y * m_iNbTilesAbs].x = x;
			m_arrPartitions[x + y * m_iNbTilesAbs].y = y;
		}
	}
}

// -----------------------------------------------------------------
// Name : ~SpacePart
// -----------------------------------------------------------------
SpacePart::~SpacePart()
{
	delete[] m_arrPartitions;
}

// -----------------------------------------------------------------
// Name : put
//	Complexity o(1)
// -----------------------------------------------------------------
void SpacePart::put(PartitionableItem * pItem)
{
	// Determine partition to use (o(1))
	f3d pos = pItem->getPosition();
	int i = (int) (m_iNbTilesAbs * pos.x / m_Dimensions.x);
	int j = (int) (m_iNbTilesOrd * pos.y / m_Dimensions.y);
	if (i < 0) {
		i = 0;
	} else if (i >= m_iNbTilesAbs) {
		i = m_iNbTilesAbs - 1;
	}
	if (j < 0) {
		j = 0;
	} else if (j >= m_iNbTilesOrd) {
		j = m_iNbTilesOrd - 1;
	}

	// Replace previous partition
	Partition * newPartition = &(m_arrPartitions[i + j * m_iNbTilesAbs]);
	Partition * oldPartition = pItem->getPartition();
	if (newPartition != oldPartition) {
		if (oldPartition != NULL) {
			oldPartition->partition.remove(pItem);
		}
		newPartition->partition.push_back(pItem);
		pItem->setPartition(newPartition);
	}
}

// -----------------------------------------------------------------
// Name : getDirectNeighbours
// -----------------------------------------------------------------
list<PartitionableItem*> * SpacePart::getDirectNeighbours(PartitionableItem * pItem)
{
	return &(pItem->getPartition()->partition);
}

// -----------------------------------------------------------------
// Name : getIndirectNeighbours
// -----------------------------------------------------------------
list<PartitionableItem*> * SpacePart::getIndirectNeighbours(PartitionableItem * pItem, int iteration)
{
	if (iteration >= 4) {
		// skip iteration 4
		iteration++;
	}
	int dx = (iteration % 3) - 1;	// -1, 0, 1, -1, n/a, 1, -1, 0, 1
	int dy = (iteration / 3) - 1;	// -1, -1, -1, 0, n/a, 0, 1, 1, 1
	Partition * p = pItem->getPartition();
	int x = p->x + dx;
	int y = p->y + dy;
	if (x >= 0 && x < m_iNbTilesAbs && y >= 0 && y < m_iNbTilesOrd) {
		return &(m_arrPartitions[x + y * m_iNbTilesAbs].partition);
	} else {
		return NULL;
	}
}
