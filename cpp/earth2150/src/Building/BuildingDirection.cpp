#include "Building/BuildingDirection.h"

void BuildingDirection::convertPosition(uint8_t& x, uint8_t& y, uint8_t maxX, uint8_t maxY) const {
	if (direction == DIRECTION_NORTH)
		return;

	if (direction == DIRECTION_SOUTH) {
		y = maxY - y;
		return;
	}


	//				 _
	//  _ _ _  		|_|_
	// |_|_|_|	->	|_|_|
	// |_|_|		|_|_|
	if (direction == DIRECTION_EAST) {
		const uint8_t x_ = x;

		x = y;
		y = maxY - x_;
		return;
	}

	//				 _
	//    _ _  		|_|_
	//  _|_|_|	->	|_|_|
	// |_|_|_|		|_|_|
	// direction == DIRECTION_WEST
	const uint8_t x_ = x;

	x = maxX - y;
	y = x_;
}
