#ifndef MAPMANIPULATOR_H_INCLUDED
#define MAPMANIPULATOR_H_INCLUDED

#include <stdint.h>

class Map;
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
		void setHeight(const MapRectArea& area, uint16_t height);

		/// Bestimmt die minimale Höhe die in dem Gebiet vorkommt
		uint16_t getMinHeight(const MapRectArea& area) const;

		/// Bestimmt die maximale Höhe die in dem Gebiet vorkommt
		uint16_t getMaxHeight(const MapRectArea& area) const;

		/// Bestimmt die durchschnittliche Höhe innerhalb des Gebiets
		uint16_t getAvgHeight(const MapRectArea& area) const;

};


#endif // MAPMANIPULATOR_H_INCLUDED
