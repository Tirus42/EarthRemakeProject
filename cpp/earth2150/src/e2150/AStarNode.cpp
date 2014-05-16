#include "e2150/AStarNode.h"

static const uint16_t MULTIPLICATOR = 5741; // 985, 169, 99, 29;
static const uint16_t SQRT_1 = MULTIPLICATOR;
static const uint16_t SQRT_2 = M_SQRT2 * MULTIPLICATOR;

uint32_t AStarNode::farDistance(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2){
	uint16_t x = (x1 < x2) ? x2 - x1 : x1 - x2;
	uint16_t y = (y1 < y2) ? y2 - y1 : y1 - y2;
	return (x < y) ? (SQRT_2 - SQRT_1) * x + SQRT_1 * y : (SQRT_2 - SQRT_1) * y + SQRT_1 * x;
}

uint32_t AStarNode::nearDistance(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
	return (x1 == x2 || y1 == y2) ? SQRT_1 : SQRT_2;
}

AStarNode::AStarNode(AStarNode *previousNode, uint32_t spentCost, uint32_t estimationCost, uint32_t position):
		previousNode(previousNode),
		spentCost(spentCost),
		estimationCost(estimationCost),
		position(position),
		removed(false){
}
