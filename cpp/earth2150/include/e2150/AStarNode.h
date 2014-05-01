#ifndef ASTARNODE_H
#define ASTARNODE_H

#include "Map/MapPosition.h"

#ifndef M_SQRT2
#define M_SQRT2 1.41421356237309504880
#endif

class AStarNode{
	private:
		static const uint16_t MULTIPLICATOR = 5741; // 985, 169, 99, 29;
		static const uint16_t SQRT_1 = MULTIPLICATOR;
		static const uint16_t SQRT_2 = M_SQRT2 * MULTIPLICATOR;

		AStarNode *previousNode;
		uint32_t spentCost;
		uint32_t estimationCost;
		uint32_t position;
		bool old;

		AStarNode(const AStarNode& cc);
		AStarNode& operator=(const AStarNode& cc);
	public:
		static uint32_t farDistance(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
		static uint32_t nearDistance(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2){ return (x1==x2 || y1==y2) ? SQRT_1 : SQRT_2; }

		AStarNode(AStarNode *previousNode, uint32_t spentCost, uint32_t estimationCost, uint32_t position);

		uint32_t getEstimationCost() {
			return estimationCost;
		}

		AStarNode *getPreviousNode() const{return previousNode;}
		uint32_t getSpentCost() const{return spentCost;}
		uint32_t getPosition() const{return position;}
		void setOld(){old=true;}
		bool isOld() const{return old;}
};

#endif // ASTARNODE_H
