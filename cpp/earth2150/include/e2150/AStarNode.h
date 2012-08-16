#ifndef ASTARNODE_H
#define ASTARNODE_H

#include "e2150/MapPosition.h"
#include <cmath>

class AStarNode{
	private:
		static const uint32_t MULTIPLICATOR;
		static const uint32_t SQRT_1;
		static const uint32_t SQRT_2;

		AStarNode *previousNode;
		uint32_t overallCost;
		uint32_t spentCost;
		uint32_t estimationCost;
		uint32_t x;
		uint32_t y;
	public:
		static uint32_t farDistance(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1);
		static uint32_t nearDistance(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1){return (x0 == x1 || y0 == y1) ? SQRT_1 : SQRT_2;}

		AStarNode(AStarNode *previousNode, uint32_t spentCost, uint32_t estimationCost, uint32_t x, uint32_t y);

		uint32_t getEstimationCost() {
			return estimationCost;
		}

		uint32_t getOverallCost() const{return overallCost;}
		MapPosition getPosition() const{return MapPosition(x, y);}
		AStarNode *getPreviousNode() const{return previousNode;}
		uint32_t getSpentCost() const{return spentCost;}
		uint32_t getX() const{return x;}
		uint32_t getY() const{return y;}
};

#endif // ASTARNODE_H
