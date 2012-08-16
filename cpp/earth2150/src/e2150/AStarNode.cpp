#include "e2150/AStarNode.h"

#include <cmath>

const uint32_t AStarNode::MULTIPLICATOR = 5741; // 985, 169, 99, 29
const uint32_t AStarNode::SQRT_1 = MULTIPLICATOR;
const uint32_t AStarNode::SQRT_2 = (uint32_t) (M_SQRT2 * MULTIPLICATOR);

uint32_t AStarNode::farDistance(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1){
	uint32_t xDiff = std::abs(x1 - x0);
	uint32_t yDiff = std::abs(y1 - y0);
	return (xDiff < yDiff) ? xDiff * SQRT_2 + (yDiff - xDiff) * SQRT_1 : yDiff * SQRT_2 + (xDiff - yDiff) * SQRT_1;
}

AStarNode::AStarNode(AStarNode *previousNode, uint32_t spentCost, uint32_t estimationCost, uint32_t x, uint32_t y):
		previousNode(previousNode),
		spentCost(spentCost),
		estimationCost(estimationCost),
		x(x),
		y(y){
}
