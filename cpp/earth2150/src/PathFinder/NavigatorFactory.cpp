#include "PathFinder/NavigatorFactory.h"

#include "e2150/AStar.h"
#include "e2150/JPSNavigator.h"
#include "PathFinder/TNavigator.h"

INavigator* NavigatorFactory::getNavigator(const Map& map, uint8_t typ) {
	switch (typ) {
		case NAVIGATOR_ASTAR:
			return new AStar(map);

		case NAVIGATOR_JPS:
			return new JPSNavigator(map);

		case NAVIGATOR_BSEARCH:
			return new TNavigator(map);

		default:
			return getNavigator(map, NAVIGATOR_DEFAULT);

	}
}
