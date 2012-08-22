#ifndef ENTITY_H
#define ENTITY_H

#include "e2150/MapPosition.h"

class Entity {
	private:
		uint32_t id;
		MapPosition mapPosition;

		int32_t hitPoints;		// Lebenspunkte (bewusst als signed-int gew�hlt)
		int32_t hitPointsMax;	//Maximale Lebenspunkte des Objekts

	public:
		Entity();
		Entity(uint32_t id, int32_t hitPointsMax);
		virtual ~Entity();

		/// Gibt die Entity-ID zur�ck
		uint32_t getID() const {return id;}

		/// Gibt die Position zur�ck
		uint32_t getX() const {return mapPosition.getX();}
		uint32_t getY() const {return mapPosition.getY();}

		/// Setzt die Position auf der Karte
		void setPosition(uint16_t x, uint16_t y) {mapPosition.setXY(x, y);}

		/// Gibt den aktuellen HP-Wert zur�ck
		int32_t getHitPoints() const {return hitPoints;}

		/// Gibt den Maximalen HP-Wert des Objekts zur�ck
		int32_t getHitPointsMax() const {return hitPointsMax;}

		/// Schreibt alle relevanten Informationen �ber das Objekt in den Buffer und gibt die gr��e zur�ck
		int32_t dumpData(char* buffer) const;
};

#endif
