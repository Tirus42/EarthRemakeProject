#ifndef ASTAR_H
#define ASTAR_H

#include "e2150/Navigator.h"

class AStarNode;

class AStar: public Navigator{
	public:
		AStar(const Map& map): Navigator(map){}

		bool getPath(uint32_t start_index, uint32_t goal_index, std::list<uint32_t>& path_list) const;
};

#endif
