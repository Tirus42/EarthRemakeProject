#include "e2150/AStarBinaryHeap.h"

AStarBinaryHeap::AStarBinaryHeap(uint32_t spaceForNodes):
		nodes(),
		knownPositons(){
	nodes.push_back(AStarNode(0, 0, 0, 0, 0)); //will never be used
}

void AStarBinaryHeap::add(const AStarNode& node){
	knownPositons.insert(node.getPosition());
	nodes.push_back(node);
	uint32_t m = nodes.size()-1;
	while (m != 1) {
		uint32_t m2 = m >> 1;
		if (nodes[m].getOverallCost() <= nodes[m2].getOverallCost()) {
			std::swap(nodes[m2], nodes[m]);
			m = m2;
		} else {
			break;
		}
	}
}

const AStarNode *AStarBinaryHeap::getNode(const MapPosition& position) const{
	if (knownPositons.find(position)!=knownPositons.end()) {
		for (std::vector<AStarNode>::const_iterator node=nodes.begin()+1;node!=nodes.end();node++) {
			if (node->getX()==position.getX() && node->getY()==position.getY()) {
				return &*node;
			}
		}
	}
	return 0;
}

AStarNode AStarBinaryHeap::poll(){
	AStarNode _ = nodes[1];
	knownPositons.erase(nodes[1].getPosition());
	nodes[1] = *(nodes.end()-1);
	uint32_t v = 1;
	while (true) {
		uint32_t u = v;
		if (2 * u + 1 <= nodes.size()) {
			if (nodes[u].getOverallCost()>=nodes[2 * u].getOverallCost()) {
				v = 2 * u;
			}
			if (nodes[v].getOverallCost() >= nodes[2 * u + 1].getOverallCost()) {
				v = 2 * u + 1;
			}
		} else if (2 * u <= nodes.size()) {
			if (nodes[u].getOverallCost() >= nodes[2 * u].getOverallCost()) {
				v = 2 * u;
			}
		}
		if (u != v) {
			std::swap(nodes[u], nodes[v]);
		} else {
			break;
		}
	}
	return _;
}
