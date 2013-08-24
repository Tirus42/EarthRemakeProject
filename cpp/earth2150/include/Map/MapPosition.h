#ifndef MAPPOSITION_H_INCLUDED
#define MAPPOSITION_H_INCLUDED

#include <stdint.h>

/**
* Stellt eine Position auf einer Karte da
*/
class MapPosition {
	private:
		uint16_t x;
		uint16_t y;

	public:
		MapPosition() : x(~0), y(~0) {};
		MapPosition(uint16_t x, uint16_t y) : x(x), y(y) {};

		bool operator<(const MapPosition& cc) const {
			return x < cc.getX() ? true : (x > cc.getX() ? false : (y < cc.getY()));
		}

		/// Getter
		uint16_t getX() const {return x;}
		uint16_t getY() const {return y;}

		/// Setter
		void setX(uint16_t x) {this->x = x;}
		void setY(uint16_t y) {this->y = y;}
		void setXY(uint16_t x, uint16_t y) {this->x = x; this->y = y;}

		/// Prüft ob eine Position gesetzt wurde
		bool isValid() {
			return (x != (uint16_t)(~0) && y != (uint16_t)(~0));
		}

		/// Gibt eine ungültige Position zurück (isValid == false)
		static MapPosition InvalidPosition() {
			return MapPosition();
		}
};

#endif
