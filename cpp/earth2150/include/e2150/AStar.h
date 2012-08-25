#ifdef false
#ifndef ASTAR_H
#define ASTAR_H

#include "e2150/Navigator.h"

class AStar: public Navigator{
	public:
		AStar();

		std::vector<MapPosition> getPath(const Map& map, uint16_t startX, uint16_t startY, uint16_t x, uint16_t y) const;
};

#endif // ASTAR_H
#endif
