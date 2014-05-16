#include "e2150/AStar.h"

#include "e2150/AStarNode.h"
#include "Map/Map.h"
#include "Map/MapBitLayer.h"
#include "Unit/Unit.h"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstring>
#include <queue>

bool AStar::buildPathAndEraseRAM(AStarNode *currentNode, const std::map<uint32_t, std::vector<AStarNode*> >& openList, const std::deque<AStarNode*>& gc, std::list<uint32_t>& path_list) const{
	if (currentNode){
		for (AStarNode *node=currentNode;node;node=node->getPreviousNode()){
			assert(!currentNode->isRemoved());
			path_list.push_front(node->getPosition());
		}
		path_list.pop_front();
	}
	for(std::map<uint32_t, std::vector<AStarNode*> >::const_iterator i=openList.begin();i!=openList.end();i++){
		for(std::vector<AStarNode*>::const_iterator j=i->second.begin();j!=i->second.end();j++){
			delete *j;
		}
	}
	for (std::deque<AStarNode*>::const_iterator node=gc.begin();node!=gc.end();node++) {
		delete *node;
	}
	if (currentNode){
		delete currentNode;
		return true;
	}
	return false;
}

bool AStar::getPath(uint32_t start_index, uint32_t goal_index, std::list<uint32_t>& path_list) const {
	uint16_t destinationX = map.positionX(goal_index);
	uint16_t destinationY = map.positionY(goal_index);
	std::map<uint32_t, std::vector<AStarNode*> > openList;
	const uint16_t MAP_WIDTH = map.getWidth();
	const uint16_t MAP_HEIGHT = map.getHeight();

	std::vector<AStarNode*> nodePositions(MAP_WIDTH * MAP_HEIGHT);
	MapBitLayer closedList(0, 0, MAP_WIDTH, MAP_HEIGHT); //map.getMinX(), map.getMinY()
	std::deque<AStarNode*> gc; //garbage collector

	AStarNode *firstNode = new AStarNode(0, 0, 0, start_index);
	openList[0].push_back(firstNode);
	nodePositions[start_index] = firstNode;

	do {
		std::map<uint32_t, std::vector<AStarNode*> >::iterator openListBegin = openList.begin();
		AStarNode *currentNode = *openListBegin->second.rbegin();
		if (openListBegin->second.size() != 1) {
			openListBegin->second.pop_back();
		} else {
			openList.erase(openListBegin);
		}
		if (!currentNode->isRemoved()) {
			const uint32_t CURRENT_POSITION = currentNode->getPosition();
			if (CURRENT_POSITION == goal_index) {
				return buildPathAndEraseRAM(currentNode, openList, gc, path_list);
			}
			const uint16_t CURRENT_X = map.positionX(CURRENT_POSITION);
			const uint16_t CURRENT_Y = map.positionY(CURRENT_POSITION);
			closedList.set(CURRENT_X, CURRENT_Y);

			uint32_t neighbours[8];
			size_t numberOfNeighbours = map.getNeighbours(CURRENT_POSITION, neighbours);
			for (size_t i=0;i!=numberOfNeighbours;i++) {
				const uint32_t neighbour = neighbours[i];
				const uint16_t NEIGHBOUR_X = map.positionX(neighbour);
				const uint16_t NEIGHBOUR_Y = map.positionY(neighbour);
				if (closedList.isSet(NEIGHBOUR_X, NEIGHBOUR_Y)) {
					continue;
				}
				uint32_t neighbourSpentCost = currentNode->getSpentCost()+AStarNode::nearDistance(CURRENT_X, CURRENT_Y, NEIGHBOUR_X, NEIGHBOUR_Y);
				AStarNode *node = nodePositions[neighbour];
				if (node && neighbourSpentCost < node->getSpentCost()) {
					node->remove(); //no delete, will be double on list but doesn't matter :)
					node = 0;
				}
				if (!node) {
					uint32_t neigbourEstimationCost = AStarNode::farDistance(destinationX, destinationY, NEIGHBOUR_X, NEIGHBOUR_Y);
					AStarNode *newNode = new AStarNode(
						currentNode, neighbourSpentCost,
						neigbourEstimationCost,
						neighbour
					);
					openList[neighbourSpentCost + neigbourEstimationCost].push_back(newNode);
					nodePositions[neighbour] = newNode;
				}
			}
		}
		gc.push_back(currentNode);
	} while (!openList.empty());
	return buildPathAndEraseRAM(0, openList, gc, path_list);
}
