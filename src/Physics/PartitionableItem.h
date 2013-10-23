#ifndef _PARTITIONABLE_ITEM_H
#define _PARTITIONABLE_ITEM_H

#include "../Utils/utils.h"

class Partition;

class PartitionableItem
{
public:
	PartitionableItem() { m_pPartition = NULL; };
	virtual ~PartitionableItem() {};
	virtual f3d getPosition() const = 0;
	Partition * getPartition() { return m_pPartition; };
	void setPartition(Partition * pPartition) { m_pPartition = pPartition; };

private:
	Partition * m_pPartition;
};

#endif
