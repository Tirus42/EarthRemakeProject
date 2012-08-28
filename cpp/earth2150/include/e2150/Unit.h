#ifndef UNIT_H
#define UNIT_H

#include "e2150/Entity.h"

class UnitChassis;

class Unit : public Entity {
	private:
		/// Beschreibt den Status der Einheit, siehe FLAG_ Konstanten
		uint8_t statusFlags;

		/// Die Chassis aus welcher die Einheit besteht
		const UnitChassis& chassis;

	public:
		static const uint8_t FLAG_INBUILDING	= (1 << 0);
		static const uint8_t FLAG_ONMOVE		= (1 << 1);

		Unit(uint32_t id, const UnitChassis& chassis);
		virtual ~Unit();

		/**
		* Gibt an ob auf Spielerbefehle direkt reagiert werden kann
		* Nicht der fall wenn Einheit aus Werkshalle fährt, unter einfluss von Signalstörern o.ä. steht
		*/
		bool isControlable() const;

		/// Setzt ein Status-Bit für das angegebene Feld
		void setStatusFlag(uint8_t statusFlag, bool value);

		/// Gibt das Status-Bit des angegebenen Feldes zurück
		bool getStatusFlag(uint8_t statusFlag) const;

		/// Gibt das Chassis zurück, aus dem die Einheit besteht
		const UnitChassis& getChassis() const {return chassis;}

		/// Bewegt die Einheit ein Feld in die entsprechende Richtung
		void move(uint8_t direction);

		int32_t dumpData(char* buffer) const;
};

#endif
