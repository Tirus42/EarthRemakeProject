#ifndef ASTARNODE_H
#define ASTARNODE_H

#include "e2150/MapPosition.h"

class AStarNode{
	private:
		static const uint16_t MULTIPLICATOR;
		static const uint16_t SQRT_1;
		static const uint16_t SQRT_2;

		AStarNode *previousNode;
		uint32_t spentCost;
		uint32_t estimationCost;
		uint32_t position;
		bool old;

		AStarNode(const AStarNode& cc);
		AStarNode& operator=(const AStarNode& cc);
	public:
		static uint32_t farDistance(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
		static uint32_t nearDistance(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1){return (x0==x1 || y0==y1)?SQRT_1:SQRT_2;}

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
