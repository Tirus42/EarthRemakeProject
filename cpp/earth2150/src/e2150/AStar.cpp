#include "e2150/AStar.h"

#include "e2150/AStarBinaryHeap.h"
#include "e2150/Map.h"
#include "e2150/Unit.h"
#include <algorithm>
#include <cmath>
#include <set>

AStar::AStar(){
}

std::vector<MapPosition> AStar::getPath(const Map& map, uint16_t startX, uint16_t startY, uint16_t x, uint16_t y) const {
	uint32_t expectedSizeOfNodes = std::abs(startX - x) + std::abs(startY - y);
	AStarBinaryHeap openList(expectedSizeOfNodes);
	std::set<MapPosition> closedList;
	openList.add(new AStarNode(0, 0, 0, startX, startY));
	do {
		AStarNode *currentNode = openList.poll();
		const uint32_t CURRENT_NODE_X = currentNode->getX();
		const uint32_t CURRENT_NODE_Y = currentNode->getY();
		if (CURRENT_NODE_X == x && CURRENT_NODE_Y == y) {
			std::vector<MapPosition> _;
			AStarNode *node=currentNode;
			while(node){
				_.push_back(node->getPosition());
				node=node->getPreviousNode();
			}
			std::reverse(_.begin(), _.end());
			return _;
		}
		closedList.insert(currentNode->getPosition());
		std::vector<MapPosition> NEIGHBOUR_POSITIONS = map.getNeighbourPositions(CURRENT_NODE_X, CURRENT_NODE_Y);
		for (std::vector<MapPosition>::iterator neighbourPosition=NEIGHBOUR_POSITIONS.begin();neighbourPosition!=NEIGHBOUR_POSITIONS.end();neighbourPosition++) {
			const uint32_t NEIGHBOUR_X = neighbourPosition->getX();
			const uint32_t NEIGHBOUR_Y = neighbourPosition->getY();
			if (closedList.find(*neighbourPosition)!=closedList.end()) {
				continue;
			}
			uint32_t neighbourSpentCost = currentNode->getSpentCost() + AStarNode::nearDistance(CURRENT_NODE_X, CURRENT_NODE_Y, NEIGHBOUR_X, NEIGHBOUR_Y);
			bool addNeighbourNode = true;
			const AStarNode *node = openList.getNode(*neighbourPosition);
			if (node != 0 && neighbourSpentCost >= node->getSpentCost()) {
				addNeighbourNode = false;
			}
			if (addNeighbourNode) {
				openList.add(new AStarNode(currentNode, neighbourSpentCost, AStarNode::farDistance(x, y, NEIGHBOUR_X, NEIGHBOUR_Y), NEIGHBOUR_X, NEIGHBOUR_Y));
			}
		}
	} while (!openList.isEmpty());
	return std::vector<MapPosition>();
}
