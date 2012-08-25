#include "e2150/AStar.h"

#include "e2150/AStarNodeComparator.h"
#include "e2150/Map.h"
#include "e2150/MapBitLayer.h"
#include "e2150/Unit.h"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstring>
#include <queue>

bool AStar::buildPathAndEraseRAM(AStarNode *currentNode, std::priority_queue<AStarNode*, std::vector<AStarNode*>, AStarNodeComparator>& openList, std::deque<AStarNode*>& gc, std::list<uint32_t>& path_list) const{
	for(AStarNode *node=currentNode;node;node=node->getPreviousNode()){
		assert(!currentNode->isOld());
		path_list.push_front(map.position(node->getX(), node->getY()));
	}
	while(!openList.empty()){
		delete openList.top();
		openList.pop();
	}
	for (std::deque<AStarNode*>::const_iterator node=gc.begin();node!=gc.end();node++) {
		delete *node;
	}
	if(currentNode){
		delete currentNode;
		return true;
	}
	return false;
}

bool AStar::getPath(uint32_t start_index, uint32_t goal_index, std::list<uint32_t>& path_list) const{
	uint16_t startX=map.positionX(start_index);
	uint16_t startY=map.positionY(start_index);
	uint16_t destinationX=map.positionX(goal_index);
	uint16_t destinationY=map.positionY(goal_index);
	std::priority_queue<AStarNode*, std::vector<AStarNode*>, AStarNodeComparator> openList;
	const uint16_t MAP_WIDTH=map.getWidth();
	const uint16_t MAP_HEIGHT=map.getHeight();

	AStarNode **nodePositions=new AStarNode*[MAP_WIDTH*MAP_HEIGHT];
	std::memset(nodePositions, 0, MAP_WIDTH*MAP_HEIGHT*2);
	MapBitLayer closedList(0, 0, MAP_WIDTH, MAP_HEIGHT); //map.getMinX(), map.getMinY()
	std::deque<AStarNode*> gc;

	AStarNode *firstNode=new AStarNode(0, 0, 0, startX, startY);
	openList.push(firstNode);
	nodePositions[startY*MAP_WIDTH+startX]=firstNode;

	do{
		AStarNode *currentNode=openList.top();
		openList.pop();
		if(!currentNode->isOld()){
			const uint16_t CURRENT_NODE_X=currentNode->getX();
			const uint16_t CURRENT_NODE_Y=currentNode->getY();
			if(CURRENT_NODE_X==destinationX && CURRENT_NODE_Y==destinationY) {
				delete[] nodePositions;
				return buildPathAndEraseRAM(currentNode, openList, gc, path_list);
			}
			closedList.set(CURRENT_NODE_X, CURRENT_NODE_Y);

			std::vector<MapPosition> NEIGHBOUR_POSITIONS = map.getNeighbourPositions(CURRENT_NODE_X, CURRENT_NODE_Y);
			for(std::vector<MapPosition>::const_iterator neighbourPosition=NEIGHBOUR_POSITIONS.begin();neighbourPosition!=NEIGHBOUR_POSITIONS.end();neighbourPosition++) {
				const uint16_t NEIGHBOUR_X=neighbourPosition->getX();
				const uint16_t NEIGHBOUR_Y=neighbourPosition->getY();
				if(closedList.isSet(NEIGHBOUR_X, NEIGHBOUR_Y)){
					continue;
				}
				uint32_t neighbourSpentCost=currentNode->getSpentCost()+AStarNode::nearDistance(CURRENT_NODE_X, CURRENT_NODE_Y, NEIGHBOUR_X, NEIGHBOUR_Y);
				AStarNode *node=nodePositions[NEIGHBOUR_Y*MAP_WIDTH+NEIGHBOUR_X];
				if (node && neighbourSpentCost<node->getSpentCost()) {
					node->setOld(); //no delete, will be double on list but doesn't matter :)
					node=0;
				}
				if(!node) {
					AStarNode *newNode=new AStarNode(
						currentNode, neighbourSpentCost,
						AStarNode::farDistance(destinationX, destinationY, NEIGHBOUR_X, NEIGHBOUR_Y),
						NEIGHBOUR_X, NEIGHBOUR_Y
					);
					openList.push(newNode);
					nodePositions[NEIGHBOUR_Y*MAP_WIDTH+NEIGHBOUR_X]=newNode;
				}
			}
		}
		gc.push_back(currentNode);
	}while(!openList.empty());
	delete[] nodePositions;
	return buildPathAndEraseRAM(0, openList, gc, path_list);
}
