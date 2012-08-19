#ifndef MAPPOSITION_H_INCLUDED
#define MAPPOSITION_H_INCLUDED

#include <stdint.h>

/**
* Stellt eine Position auf einer Karte da
*/
class MapPosition {
	private:
		uint16_t x, y;
	public:
		MapPosition() : x(0), y(0) {};
		MapPosition(uint16_t x, uint16_t y) : x(x), y(y) {};

		bool operator<(const MapPosition& cc) const{return x<cc.getX()?true:(x>cc.getX()?false:(y<cc.getY()));}

		uint16_t getX() const {return x;}
		uint16_t getY() const {return y;}

		void setX(uint16_t x) {this->x = x;}
		void setY(uint16_t y) {this->y = y;}
		void setPosition(uint16_t x, uint16_t y) {this->x = x; this->y = y;}
};

#endif
