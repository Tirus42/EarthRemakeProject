#ifndef MAPPOSITION_H_INCLUDED
#define MAPPOSITION_H_INCLUDED

#include <stdint.h>

/**
* Stellt eine Position auf einer Karte da
*/
class MapPosition {
	private:
		uint32_t x, y;
	public:
		MapPosition() : x(0), y(0) {};
		MapPosition(uint32_t x, uint32_t y) : x(x), y(y) {};

		uint32_t getX() const{return x;}
		uint32_t getY() const{return y;}
};

#endif
