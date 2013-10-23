#ifndef _SPACE_PART_H
#define _SPACE_PART_H

#include "PartitionableItem.h"

#define NB_INDIRECT_NEIGHBOURS_ZONES	8

class Partition
{
public:
	list<PartitionableItem*> partition;
	int x, y;
};

class SpacePart
{
public:
	SpacePart(f3d topLeft, f3d dimensions, double fInteractionRadius);
	~SpacePart();

	void put(PartitionableItem*);
	list<PartitionableItem*> * getDirectNeighbours(PartitionableItem*);
	list<PartitionableItem*> * getIndirectNeighbours(PartitionableItem*, int);

private:
	f3d m_TopLeft;
	f3d m_Dimensions;
	double m_fInteractionRadius;
	int m_iNbTilesAbs, m_iNbTilesOrd;
	Partition * m_arrPartitions;
};

#endif
