#pragma once

#include <stdint.h>

class Map;
class MapPosition;
class MapRectArea;

/**
* Klasse welche die Höhe sowie die Texturierung
* der Kartenoberfläche verändern kann.
* Bietet auch die Möglichkeit einige Infos über die Höhenkarte abzurufen.
*/
class MapManipulator {
	private:
		Map& map;

		MapManipulator(const MapManipulator&);
		MapManipulator operator=(const MapManipulator&);

	public:
		MapManipulator(Map& map);

		/// Setzt die Höhe in dem Gebiet
		void setHeight(const MapRectArea& area, uint16_t height) const;

		/// Bestimmt die minimale und maximale Höhe welche in dem Gebiet vorkommt
		bool getMinMaxHeight(const MapRectArea& area, uint16_t& min, uint16_t& max) const;

		/// Bestimmt die durchschnittliche Höhe innerhalb des Gebiets
		uint16_t getAvgHeight(const MapRectArea& area) const;

		/// Kopiert einen Teil der Höhenkarte von einer Karte auf die andere
		bool copyAreaHeight(const MapRectArea& from, const MapPosition& to, const Map* sourceMap = 0) const;

};
