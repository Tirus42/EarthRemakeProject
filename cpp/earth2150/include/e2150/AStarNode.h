#ifndef ASTARNODE_H
#define ASTARNODE_H

#include "e2150/MapPosition.h"

class AStarNode{
	private:
		static const uint16_t MULTIPLICATOR;
		static const uint16_t SQRT_1;
		static const uint16_t SQRT_2;

		AStarNode *previousNode;
		uint32_t overallCost;
		uint32_t spentCost;
		uint32_t estimationCost;
		uint16_t x;
		uint16_t y;
		bool old;

		AStarNode(const AStarNode& cc);
		AStarNode& operator=(const AStarNode& cc);
	public:
		static uint32_t farDistance(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
		static uint32_t nearDistance(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1){return (x0==x1 || y0==y1)?SQRT_1:SQRT_2;}

		AStarNode(AStarNode *previousNode, uint32_t spentCost, uint32_t estimationCost, uint16_t x, uint16_t y);

		uint32_t getEstimationCost() {
			return estimationCost;
		}

		uint32_t getOverallCost() const{return overallCost;}
		MapPosition getPosition() const{return MapPosition(x, y);}
		AStarNode *getPreviousNode() const{return previousNode;}
		uint32_t getSpentCost() const{return spentCost;}
		uint16_t getX() const{return x;}
		uint16_t getY() const{return y;}
		void setOld(){old=true;}
		bool isOld() const{return old;}
};

#endif // ASTARNODE_H
