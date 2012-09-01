#ifndef ASTAR_H
#define ASTAR_H

#include "e2150/AStarNodeComparator.h"
#include "e2150/Navigator.h"
#include <deque>
#include <queue>

class AStarNode;

class AStar: public Navigator{
	public:
		AStar(const Map& map): Navigator(map){}

		bool getPath(uint32_t start_index, uint32_t goal_index, std::list<uint32_t>& path_list) const;
		bool buildPathAndEraseRAM(
			AStarNode *currentNode,
			const std::map<uint32_t, std::vector<AStarNode*> >& openList,
			const std::deque<AStarNode*>& gc,
			std::list<uint32_t>& path_list
		) const;
};

#endif // ASTAR_H
