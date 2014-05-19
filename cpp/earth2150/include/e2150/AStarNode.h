#ifndef ASTARNODE_H
#define ASTARNODE_H

#include "Map/MapPosition.h"

#ifndef M_SQRT2
#define M_SQRT2 1.41421356237309504880
#endif

class AStarNode{
	private:
		uint32_t previousNodeID;
		uint32_t spentCost;
		uint32_t estimationCost;
		uint32_t position;
		bool removed;
	public:
		static uint32_t farDistance(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
		static uint32_t nearDistance(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
		static const uint32_t INVAILD_ID;

		AStarNode(uint32_t previousNodeID, uint32_t spentCost, uint32_t estimationCost, uint32_t position):
				previousNodeID(previousNodeID),
				spentCost(spentCost),
				estimationCost(estimationCost),
				position(position),
				removed(false) {
		}

		uint32_t getEstimationCost() {
			return estimationCost;
		}

		uint32_t getPreviousNode() const { return previousNodeID; }
		uint32_t getSpentCost() const { return spentCost; }
		uint32_t getPosition() const { return position; }
		void remove() { removed = true; }
		bool isRemoved() const { return removed; }
};

#endif
