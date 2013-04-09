#ifndef BUILDING_H_INCLUDED
#define BUILDING_H_INCLUDED

#include "e2150/Entity.h"

/**
* Diese Klasse stellt eine Oberklasse f�r jedliches Geb�ude dar, welches in der Spielwelt erstellt wurde.
*/
class Building : public Entity {
	protected:
		Building();

	public:
		/// Auflistung aller m�glichen Geb�udearten
		enum BuildingTyp {
			POWERSTATION,		// Kraftwerk
			HOMEBASE,			// Haputbasis / Hauptgeb�ude
			FACTORY				// Fabrik
		};

		virtual ~Building();

		/// Gibt den Typ des Geb�udes zur�ck
		virtual BuildingTyp getTyp() const = 0;

};

#endif // BUILDING_H_INCLUDED
