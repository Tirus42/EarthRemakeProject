#ifndef MAPCIRCLEAREA_H_INCLUDED
#define MAPCIRCLEAREA_H_INCLUDED

#include "Map/MapPosition.h"

class Map;

/**
* Definiert einen Kreis auf der Spielkarte.
*/
class MapCircleArea {
	private:
		/// Zentrum des Kreises
		MapPosition center;

		/// Radius des Kreises
		uint32_t radius;

	public:
		/// Standardkonstruktor, erzeugt eine ungültige Fläche
		MapCircleArea();

		/// Erzeugt einen Kreis durch angabe des Zentrums und des Radius
		MapCircleArea(const MapPosition& center, uint32_t radius);

		/// Gibt das Zentrum des Kreises zurück
		const MapPosition& getCenter() const {
			return center;
		}

		/// Gibt den Radius des Kreises zurück
		uint32_t getRadius() const {
			return radius;
		}

		/// Prüft ob das Zentrum auf der Karte liegt
		bool isCenterOnMap(const Map& map) const;
};


#endif // MAPCIRCLEAREA_H_INCLUDED
