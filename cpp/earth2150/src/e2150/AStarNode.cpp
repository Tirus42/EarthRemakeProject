#include "e2150/AStarNode.h"

uint32_t AStarNode::farDistance(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2){
	uint16_t x = (x1 < x2) ? x2 - x1 : x1 - x2;
	uint16_t y = (y1 < y2) ? y2 - y1 : y1 - y2;
	return (x < y) ? (SQRT_2 - SQRT_1) * x + SQRT_1 * y : (SQRT_2 - SQRT_1) * y + SQRT_1 * x;
}

AStarNode::AStarNode(AStarNode *previousNode, uint32_t spentCost, uint32_t estimationCost, uint32_t position):
		previousNode(previousNode),
		spentCost(spentCost),
		estimationCost(estimationCost),
		position(position),
		old(false){
}
