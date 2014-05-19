#include "e2150/AStar.h"

#include "e2150/AStarNode.h"
#include "Map/MapBitLayer.h"
#include <cassert>

static bool buildPath(uint32_t currentNodeID, const std::vector<AStarNode>& nodes, std::list<uint32_t>& path_list) {
	if (currentNodeID != AStarNode::INVAILD_ID) {
		for (uint32_t nodeID = currentNodeID; nodeID != AStarNode::INVAILD_ID; nodeID = nodes.at(nodeID).getPreviousNode()) {
			assert(!nodes.at(currentNodeID).isRemoved());
			path_list.push_front(nodes.at(nodeID).getPosition());
		}
		path_list.pop_front();
		return true;
	}
	return false;
}

bool AStar::getPath(uint32_t start_index, uint32_t goal_index, std::list<uint32_t>& path_list) const {
	uint16_t destinationX = map.positionX(goal_index);
	uint16_t destinationY = map.positionY(goal_index);
	std::map<uint32_t, std::vector<uint32_t> > openList;
	const uint16_t MAP_WIDTH = map.getWidth();
	const uint16_t MAP_HEIGHT = map.getHeight();

	std::vector<uint32_t> nodePositions(MAP_WIDTH * MAP_HEIGHT, AStarNode::INVAILD_ID);
	MapBitLayer closedList(0, 0, MAP_WIDTH, MAP_HEIGHT); //map.getMinX(), map.getMinY()
	std::vector<AStarNode> nodes;

	nodes.push_back(AStarNode(AStarNode::INVAILD_ID, 0, 0, start_index));
	openList[0].push_back(0);
	nodePositions[start_index] = 0;

	while (!openList.empty()) {
		std::map<uint32_t, std::vector<uint32_t> >::iterator openListBegin = openList.begin();
		uint32_t currentNodeID = *openListBegin->second.rbegin();
		assert(currentNodeID != AStarNode::INVAILD_ID);
		const AStarNode currentNode(nodes.at(currentNodeID));
		if (openListBegin->second.size() != 1) {
			openListBegin->second.pop_back();
		} else {
			openList.erase(openListBegin);
		}
		if (!currentNode.isRemoved()) {
			const uint32_t CURRENT_POSITION = currentNode.getPosition();
			if (CURRENT_POSITION == goal_index) {
				return buildPath(currentNodeID, nodes, path_list);
			}
			const uint16_t CURRENT_X = map.positionX(CURRENT_POSITION);
			const uint16_t CURRENT_Y = map.positionY(CURRENT_POSITION);
			closedList.set(CURRENT_X, CURRENT_Y);

			uint32_t neighbours[8];
			size_t numberOfNeighbours = map.getNeighbours(CURRENT_POSITION, neighbours);
			for (size_t i = 0; i != numberOfNeighbours; i++) {
				const uint32_t neighbour = neighbours[i];
				const uint16_t NEIGHBOUR_X = map.positionX(neighbour);
				const uint16_t NEIGHBOUR_Y = map.positionY(neighbour);
				if (closedList.isSet(NEIGHBOUR_X, NEIGHBOUR_Y)) {
					continue;
				}
				uint32_t neighbourSpentCost = currentNode.getSpentCost() + AStarNode::nearDistance(CURRENT_X, CURRENT_Y, NEIGHBOUR_X, NEIGHBOUR_Y);
				uint32_t nodeID = nodePositions[neighbour];

				if (nodeID != AStarNode::INVAILD_ID) {
					AStarNode& node = nodes.at(nodeID);
					if (neighbourSpentCost < node.getSpentCost()) {
						node.remove(); //no delete, will be double on list but doesn't matter :)
						nodeID = AStarNode::INVAILD_ID;
					}
				}
				if (nodeID == AStarNode::INVAILD_ID) {
					uint32_t neigbourEstimationCost = AStarNode::farDistance(destinationX, destinationY, NEIGHBOUR_X, NEIGHBOUR_Y);
					uint32_t newNodeID = nodes.size();
					nodes.push_back(AStarNode(
						currentNodeID, neighbourSpentCost,
						neigbourEstimationCost,
						neighbour
					));
					openList[neighbourSpentCost + neigbourEstimationCost].push_back(newNodeID);
					nodePositions[neighbour] = newNodeID;
				}
			}
		}
	}
	return false;
}
