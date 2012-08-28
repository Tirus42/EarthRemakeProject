#ifndef MOVINGUNIT_H_INCLUDED
#define MOVINGUNIT_H_INCLUDED

#include <stdint.h>

class Unit;
class Map;

/**
* Beschreibt eine sich in Bewegung befindende Einheit.
* Diese Bewegung gilt nur zwischen zwei Feldern.
* Es wird zwischen Fahrt und Drehung unterschieden.
* Objekte dieser Klasse können nach abgeschlossener Bewegung für die
* nächste Aktion wiederverwendet werden.
*/
class MovingUnit {
	private:
		/// Referenz auf die Einheit, welche sich bewegt
		Unit& unit;

		/// Ausrichtung in welche sich gedreht/gefahren werden soll
		uint8_t direction;

		/// Zeitpunkt, wann sie auf dem vorherigem Feld losgefahren ist / sich dreht
		uint32_t startTime;

		/// Beschreibt ob gedreht oder gefahren werden soll (
		bool turn;

	public:
		MovingUnit(Unit& unit, uint8_t direction, uint32_t currentTime, Map& map);
		~MovingUnit();

		/// Startet eine weitere Bewegung
		inline void startMove(uint8_t direction, uint32_t currentTime, Map& map);

		/// Schließt eine Bewegung ab (sollte nur aufgerufen werden, wenn getFinishTime() <= currentTime)
		inline void finishMove(uint32_t currentTime, Map& map);

		/// Gibt den Zeitpunkt zurück, wann die Bewegung (idr.) abgeschlossen ist
		inline uint32_t getFinishTime() const;
};

#endif // MOVINGUNIT_H_INCLUDED
