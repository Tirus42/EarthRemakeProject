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

		/// Bestimmt die minimale und maximale Höhe welche in dem Gebiet vorkommt
		bool getMinMaxHeight(const MapRectArea& area, uint16_t& min, uint16_t& max) const;

		/// Bestimmt die durchschnittliche Höhe innerhalb des Gebiets
		uint16_t getAvgHeight(const MapRectArea& area) const;

};


#endif // MAPMANIPULATOR_H_INCLUDED
