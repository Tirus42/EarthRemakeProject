#include "e2150/AStar.h"

#include "e2150/AStarBinaryHeap.h"
#include "e2150/Map.h"
#include "e2150/Unit.h"
#include <algorithm>
#include <cmath>
#include <set>

AStar::AStar(){
}

static inline std::vector<MapPosition> buildPathAndEraseRAM(AStarNode *currentNode, AStarBinaryHeap& openList, std::vector<AStarNode*>& gc){
	std::vector<MapPosition> _;
	for(AStarNode *node=currentNode;node;node=node->getPreviousNode()){
		_.push_back(node->getPosition());
	}
	std::reverse(_.begin(), _.end());
	for (std::vector<AStarNode*>::const_iterator node=openList.begin();node!=openList.end();node++) {
		delete *node;
	}
	for (std::vector<AStarNode*>::const_iterator node=gc.begin();node!=gc.end();node++) {
		delete *node;
	}
	if(currentNode){
		delete currentNode;
	}
	return _;
}

std::vector<MapPosition> AStar::getPath(const Map& map, uint16_t startX, uint16_t startY, uint16_t x, uint16_t y) const {
	uint32_t expectedSizeOfNodes = std::abs(startX - x) + std::abs(startY - y);
	AStarBinaryHeap openList(expectedSizeOfNodes);
	std::set<MapPosition> closedList;
	std::vector<AStarNode*> gc;
	gc.reserve(expectedSizeOfNodes);
	openList.add(new AStarNode(0, 0, 0, startX, startY));
	do {
		AStarNode *currentNode = openList.poll();
		const uint32_t CURRENT_NODE_X = currentNode->getX();
		const uint32_t CURRENT_NODE_Y = currentNode->getY();
		if (CURRENT_NODE_X == x && CURRENT_NODE_Y == y) {
			return buildPathAndEraseRAM(currentNode, openList, gc);
		}
		closedList.insert(currentNode->getPosition());
		std::vector<MapPosition> NEIGHBOUR_POSITIONS = map.getNeighbourPositions(CURRENT_NODE_X, CURRENT_NODE_Y);
		for (std::vector<MapPosition>::const_iterator neighbourPosition=NEIGHBOUR_POSITIONS.begin();neighbourPosition!=NEIGHBOUR_POSITIONS.end();neighbourPosition++) {
			const uint32_t NEIGHBOUR_X = neighbourPosition->getX();
			const uint32_t NEIGHBOUR_Y = neighbourPosition->getY();
			if (closedList.find(*neighbourPosition)!=closedList.end()) {
				continue;
			}
			uint32_t neighbourSpentCost = currentNode->getSpentCost() + AStarNode::nearDistance(CURRENT_NODE_X, CURRENT_NODE_Y, NEIGHBOUR_X, NEIGHBOUR_Y);
			const AStarNode *node = openList.getNode(*neighbourPosition);
			if (node == 0 || neighbourSpentCost < node->getSpentCost()) {
				openList.add(new AStarNode(currentNode, neighbourSpentCost, AStarNode::farDistance(x, y, NEIGHBOUR_X, NEIGHBOUR_Y), NEIGHBOUR_X, NEIGHBOUR_Y));
			}
		}
		gc.push_back(currentNode);
	} while (!openList.isEmpty());
	return buildPathAndEraseRAM(0, openList, gc);
}
