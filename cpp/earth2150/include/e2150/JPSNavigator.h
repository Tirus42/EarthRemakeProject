#ifndef JPS_NAVIGATOR_H_
#define JPS_NAVIGATOR_H_

#include "e2150/Navigator.h"

class JPSNavigator: public Navigator{
  public:
    std::vector<MapPosition> getPath(const Map& map, uint16_t startX, uint16_t startY, uint16_t x, uint16_t y) const;
};

#endif // JPS_NAVIGATOR_H_
