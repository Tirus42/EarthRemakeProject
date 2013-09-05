#ifndef FRACTION_H
#define FRACTION_H

#include <stdint.h>
#include <string>

/**
* Speicherstruktur für Fraktionen.
* Speichert ID und Name der Fraktion.
*/
class Faction {
	private:
		uint32_t uID;
		std::wstring name;

	public:
		Faction(uint32_t uID, const std::wstring& name);

		/// Gibt die ID der Fraktion zurück
		uint32_t getID() const {
			return uID;
		}

		/// Gibt den Namen der Fraktion zurück
		const std::wstring& getName() const {
			return name;
		}

		/// Setzt einen neuen Namen für die Fraktion
		void setName(const std::wstring& name) {
			this->name = name;
		}
};

#endif
