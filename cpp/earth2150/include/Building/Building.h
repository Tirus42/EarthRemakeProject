#ifndef BUILDING_H_INCLUDED
#define BUILDING_H_INCLUDED

#include "e2150/Entity.h"

/**
* Diese Klasse stellt eine Oberklasse für jedliches Gebäude dar, welches in der Spielwelt erstellt wurde.
*/
class Building : public Entity {
	protected:
		Building();

	public:
		/// Auflistung aller möglichen Gebäudearten
		enum BuildingTyp {
			POWERSTATION,		// Kraftwerk
			HOMEBASE,			// Haputbasis / Hauptgebäude
			FACTORY				// Fabrik
		};

		virtual ~Building();

		/// Gibt den Typ des Gebäudes zurück
		virtual BuildingTyp getTyp() const = 0;

};

#endif // BUILDING_H_INCLUDED
