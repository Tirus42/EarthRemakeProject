#include "e2150/AStarBinaryHeap.h"

AStarBinaryHeap::AStarBinaryHeap(uint32_t spaceForNodes):
		nodes(),
		positionNodes(){
	nodes.reserve(spaceForNodes);
	nodes.push_back(0); //will never be used
}

void AStarBinaryHeap::add(AStarNode *node){
	positionNodes[node->getPosition()]=node;
	nodes.push_back(node);
	uint32_t m = nodes.size()-1;
	while (m != 1) {
		uint32_t m2 = m >> 1;
		if (nodes[m]->getOverallCost() <= nodes[m2]->getOverallCost()) {
			std::swap(nodes[m2], nodes[m]);
			m = m2;
		} else {
			break;
		}
	}
}

AStarNode *AStarBinaryHeap::getNode(const MapPosition& position) const{
	std::map<MapPosition, AStarNode*>::const_iterator foundNode=positionNodes.find(position);
	return (foundNode!=positionNodes.end())?foundNode->second:0;
}

AStarNode *AStarBinaryHeap::poll() {
	AStarNode *_ = nodes[1];
	positionNodes.erase(_->getPosition());
	nodes[1] = *(nodes.end()-1);
	nodes.pop_back();
	uint32_t v = 1;
	while (true) {
		uint32_t u = v;
		if (2*u+1 <= nodes.size()) {
			if (nodes[u]->getOverallCost() >= nodes[2*u]->getOverallCost()) {
				v=2*u;
			}
			if (nodes[v]->getOverallCost() >= nodes[2*u+1]->getOverallCost()) {
				v=2*u+1;
			}
		} else if (2*u <= nodes.size()) {
			if (nodes[u]->getOverallCost() >= nodes[2*u]->getOverallCost()) {
				v = 2 * u;
			}
		}
		if (u!=v) {
			std::swap(nodes[u], nodes[v]);
		} else {
			break;
		}
	}
	return _;
}

