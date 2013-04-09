#ifndef ENTITY_H
#define ENTITY_H

#include "Map/MapPosition.h"

class Entity {
	private:
		uint32_t id;
		MapPosition mapPosition;

		/// Ausrichtung des Objekts
		uint8_t direction;

		int32_t hitPoints;		// Lebenspunkte (bewusst als signed-int gewählt)
		int32_t hitPointsMax;	//Maximale Lebenspunkte des Objekts

	public:
		Entity();
		Entity(uint32_t id, int32_t hitPointsMax);
		virtual ~Entity();

		/// Gibt die Entity-ID zurück
		uint32_t getID() const {return id;}

		/// Gibt die Position zurück
		uint32_t getX() const {return mapPosition.getX();}
		uint32_t getY() const {return mapPosition.getY();}

		/// Gibt die Ausrichtung zurück
		uint8_t getDirection() const {return direction;}

		/// Setzt die Position auf der Karte
		void setPosition(uint16_t x, uint16_t y) {mapPosition.setXY(x, y);}
		void setPosition(uint16_t x, uint16_t y, uint8_t direction) {mapPosition.setXY(x, y);
																	this->direction = direction;}

		/// Setzt nur die Ausrichtung des Objekts
		void setDirection(uint8_t direction) {this->direction = direction;}

		/// Gibt den aktuellen HP-Wert zurück
		int32_t getHitPoints() const {return hitPoints;}

		/// Gibt den Maximalen HP-Wert des Objekts zurück
		int32_t getHitPointsMax() const {return hitPointsMax;}

		/// Schreibt alle relevanten Informationen über das Objekt in den Buffer und gibt die größe zurück
		virtual int32_t dumpData(char* buffer) const;
		virtual uint32_t dumpDataSize() const;
};

#endif
