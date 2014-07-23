#ifndef MOUSEPICKER_H_INCLUDED
#define MOUSEPICKER_H_INCLUDED

#include "Map/MapPosition.h"

#include <irrlicht.h>

class VisualMap;

/**
* Klasse mit welcher ein Strahlschnitt zwischen der Mausposition auf dem Bildschirm
* gegen die Spielfeldkarte und dessen Objekte durchgeführt werden kann.
* Es wird das Objekt/Feld gefunden, worüber die Maus auf dem Bildschirm ist.
* Das Ergebnis kann daraufhin abgefragt werden.
*/
class MousePicker {
	public:
		enum PickedType {
			PickedNothing,	/// Nichts getroffen
			PickedGround,	/// Kartenboden getroffen
			PickedUnit,		/// Einheit getroffen
			PickedBuilding,	/// Gebäude getroffen
		};

		MousePicker(const VisualMap& map, irr::scene::ISceneCollisionManager* colmgr);
		~MousePicker();

		/// Gibt true zurück, wenn etwas anderes als beim letzten aufruf getroffen wurde
		bool pick(irr::core::position2di& mousePosition);

		/// Gibt die Kartenposition zurück, welche als letztes getroffen wurde
		const MapPosition& getPickedPosition() const {
			return pickedPosition;
		};

		/// Gibt den Typ des Objekts zurück das zuletzt getroffen wurde
		PickedType getPickedType() const {
			return pickedType;
		}

	private:
		const VisualMap& map;
		irr::scene::ISceneCollisionManager* colmgr;

		/// Position welche als letztes gepickt wurde
		MapPosition pickedPosition;

		/// Typ welcher gepickt wurde
		PickedType pickedType;

		MousePicker(const MousePicker&);
		MousePicker operator=(const MousePicker&);
};

#endif // MOUSEPICKER_H_INCLUDED
