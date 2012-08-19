#ifndef ASTAR_H
#define ASTAR_H

#include "e2150/Navigator.h"

class AStar: public Navigator{
	public:
		AStar();


		std::vector<MapPosition> getPath(const Map& map, const Unit& unit, uint32_t x, uint32_t y) const;
		std::vector<MapPosition> getPath(const Map& map, uint32_t start, uint32_t target) const;
		std::vector<MapPosition> getPath(const Map& map, uint32_t startX, uint32_t startY, uint32_t x, uint32_t y) const;
};

#endif // ASTAR_H
