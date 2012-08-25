#include "e2150/JPSNavigator.h"

#include "ff/JumpPointSearch.h"

std::vector<MapPosition> JPSNavigator::getPath(const Map& map,
                                               uint16_t startX, uint16_t startY,
                                               uint16_t x, uint16_t y) const {
  ff::JumpPointSearch jps(map);
  std::vector<MapPosition> path;
  std::list<ff::coord_t> path_list;

  if (jps.Solve(path_list, startX, startY, x, y)) {
    for (std::list<ff::coord_t>::iterator i = path_list.begin(); i != path_list.end(); i++) {
      path.push_back(MapPosition(i->x, i->y));
    }
  }
  return path;
}
