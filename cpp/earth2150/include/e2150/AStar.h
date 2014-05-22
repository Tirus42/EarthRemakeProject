#ifndef ASTAR_H
#define ASTAR_H

#include "PathFinder/INavigator.h"

class AStarNode;

class AStar: public INavigator{
	public:
		AStar(const Map& map): INavigator(map){}

		bool getPath(uint32_t start_index, uint32_t goal_index, std::list<uint32_t>& path_list) const;
};

#endif
