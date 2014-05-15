#ifndef BUILDINGREFERENCE_H_INCLUDED
#define BUILDINGREFERENCE_H_INCLUDED

#include "Map/MapRectArea.h"

/**
* Einfache Referenz auf den Eintrag, welcher die konkreten Gebäudeinformationen
* enthält.
*/
class BuildingReference {
	private:
		uint32_t id;

		/// Referenz ID auf den Eintrag in der Gebäudeliste
		uint32_t refID;

		/// Die Grundfläche des Gebäudes (muss nicht vollständig genutzt sein)
		MapRectArea buildingArea;

	public:
		BuildingReference();
		BuildingReference(uint32_t id, uint32_t refID, const MapRectArea& area);
		~BuildingReference();

		/// Gibt die ID zurück
		uint32_t getID() const {
			return id;
		}

		/// Gibt die Referenz ID zurück
		uint32_t getRefID() const {
			return refID;
		}

		/// Setzt eine neue Referenz ID
		void setRefID(uint32_t newRefID) {
			this->id = newRefID;
		}

		/// Die Grundfläche des Gebäudes
		const MapRectArea& BuildingArea() const {
			return buildingArea;
		}

		/// Die Grundfläche des Gebäudes
		MapRectArea& BuildingArea() {
			return buildingArea;
		}

};


#endif // BUILDINGREFERENCE_H_INCLUDED
