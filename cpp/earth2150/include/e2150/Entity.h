#ifndef ENTITY_H
#define ENTITY_H

#include "e2150/MapPosition.h"

class Entity {
	private:
		uint32_t id;
		MapPosition mapPosition;

		int32_t hitPoints;	// Lebenspunkte (bewusst als signed-int gewählt)
		int32_t hitPointsMax;	//Maximale Lebenspunkte des Objekts

	public:
		Entity();
		Entity(uint32_t id, int32_t hitPointsMax);
		virtual ~Entity();

		uint32_t getID() const {return id;}

		uint32_t getX() const {return mapPosition.getX();}
		uint32_t getY() const {return mapPosition.getY();}

		void setPosition(uint16_t x, uint16_t y) {mapPosition.setPosition(x, y);}

		uint32_t getHitPoints() const {return hitPoints;}
};

#endif
