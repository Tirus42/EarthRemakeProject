#ifndef ASTARBINARYHEAP_H
#define ASTARBINARYHEAP_H

#include "e2150/AStarNode.h"
#include "e2150/MapPosition.h"
#include <map>
#include <vector>

class AStarBinaryHeap{
	private:
		std::vector<AStarNode*> nodes;
		std::map<MapPosition, AStarNode*> positionNodes;
	public:
		AStarBinaryHeap(uint32_t spaceForNodes);

		void add(AStarNode *node);
		void clear(){nodes.clear(); positionNodes.clear();}
		AStarNode *getNode(const MapPosition& position) const;
		bool isEmpty() const{return nodes.empty();}
		AStarNode *poll();
		uint32_t size() const{return nodes.size();}
};

#endif // ASTARBINARYHEAP_H
