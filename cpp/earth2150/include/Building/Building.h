#ifndef BUILDING_H_INCLUDED
#define BUILDING_H_INCLUDED

#include "e2150/Entity.h"

/**
* Diese Klasse stellt eine Oberklasse für jegliches Gebäude dar, welches in der Spielwelt erstellt wurde.
*/
class Building : public Entity {
	private:
		const BuildingTyp typ;

	protected:
		Building(BuildingTyp typ): typ(typ) {}

	public:
		/// Auflistung aller möglichen Gebäudearten
		enum BuildingTyp {
			POWERSTATION,		// Kraftwerk
			HOMEBASE,			// Hauptbasis / Hauptgebäude
			FACTORY				// Fabrik
		};

		virtual ~Building();

		/// Gibt den Typ des Gebäudes zurück
		BuildingTyp getTyp() const { return typ; };

};

#endif // BUILDING_H_INCLUDED
