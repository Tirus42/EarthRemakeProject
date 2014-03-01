#ifndef BUILDINGDIRECTION_H_INCLUDED
#define BUILDINGDIRECTION_H_INCLUDED

#include <stdint.h>

/**
* Speichert die Ausrichtung eines Gebäudes
* und bietet die Funktion um die Feldposition je nach
* Ausrichtung umzurechnen.
*/
class BuildingDirection
{
	private:
		uint8_t direction;

	public:
		enum Building_Direction {
			DIRECTION_NORTH,
			DIRECTION_EAST,
			DIRECTION_SOUTH,
			DIRECTION_WEST
		};

		BuildingDirection(Building_Direction direction) :
			direction(direction) {}
		~BuildingDirection() {};

		Building_Direction getDirection() const {
			return (Building_Direction)direction;
		}

		/// Rechnet die Position entsprechend der Ausrichtung um (Ausrichtung auf Karte -> Nördliche Ausrichtung)
		void convertPosition(uint8_t& x, uint8_t& y, uint8_t maxX, uint8_t maxY) const;


};



#endif // BUILDINGDIRECTION_H_INCLUDED
