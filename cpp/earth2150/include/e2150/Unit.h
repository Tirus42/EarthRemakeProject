#ifndef UNIT_H
#define UNIT_H

#include "e2150/Entity.h"

class UnitChassis;

class Unit : public Entity {
	private:
		static const int FLAG_INBUILDING = 1;

		uint8_t controlableFlags;
		const UnitChassis& chassis;


	public:
		Unit(uint32_t id, const UnitChassis& chassis);
		virtual ~Unit();

		/**
		* Gibt an ob auf Spielerbefehle direkt reagiert werden kann
		* Nicht der fall wenn Einheit aus Werkshalle fährt, unter einfluss von Signalstörern steht ect
		*/
		bool isControlable() const {return !controlableFlags;}

		int32_t dumpData(char* buffer) const;
};

#endif
