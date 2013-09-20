#ifndef MAPRECTAREA_H_INCLUDED
#define MAPRECTAREA_H_INCLUDED

#include "Map/MapPosition.h"

/**
* Definiert einen Rechteckigen Bereich auf der Spielkarte.
* Es werden Methoden bereitgestellt, um zu Prüfen ob Punkte
* oder Bereiche innerhalb dieses Bereiches liegen.
*/
class MapRectArea {
	private:
		/// Erste Ecke, die kleinere im Wertebereich
		MapPosition minEdge;

		/// Zweite Position, ist >= der ersten Ecke
		MapPosition maxEdge;

		/// Stellt sicher, dass minEdge < maxEdge ist (X und Y)
		void repair();

	public:
		/// Standardkonstruktur, erzeugt eine ungültige Fläche!
		MapRectArea();

		/// Definiert die Fläche durch zwei Eckpunkte (Reihenfolge ist beliebig)
		MapRectArea(const MapPosition& pos1, const MapPosition& pos2);

		/// Definiert die Fläche durch eine MinEcke sowie die Breite und Länge
		MapRectArea(const MapPosition& pos, uint16_t width, uint16_t height);

		/// Destruktor
		~MapRectArea();

		/// Gibt die Ecke der kleineren Koorinaten zurück
		const MapPosition& getMinEdge() const {
			return minEdge;
		}

		/// Gibt die Ecke der größreren Koordinaten zurück
		const MapPosition& getMaxEdge() const {
			return maxEdge;
		}

		/// Gibt die Breite zurück (X-Achse)
		uint16_t getWidth() const {
			return maxEdge.getX() - minEdge.getX();
		}

		/// Gibt die Höhe zurück (Y-Achse)
		uint16_t getHeight() const {
			return maxEdge.getY() - minEdge.getY();
		}

		/// Gibt die Anzahl an Feldern zurück (Die Fläche)
		uint32_t getFieldCount() const {
			return (uint32_t)getWidth() * (uint32_t)getHeight();
		}

		/// Prüft ob eine Position innerhalb ist
		bool isInside(const MapPosition& position) const;

		/// Prüft ob eine andere Fläche komplett innerhalb dieser ist
		bool isInside(const MapRectArea& area) const;

};


#endif // MAPRECTAREA_H_INCLUDED
