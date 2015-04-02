#pragma once

#include <vector>
#include <stdint.h>

/**
* Hilfsklasse zum vergeben eindeutiger UID's.
* Versucht die UID's möglichst am Stück zu halten,
* in dem wieder freigegebene UID's erneut vergeben werden.
* Dafür wird der Bereich der vergebenen UID's gespeichert,
* und versucht in diesem die Lücken zu schließen.
*/
class UIDManager {
	private:
		struct UIDRange {
			uint32_t first;
			uint32_t last;

			UIDRange(uint32_t element);
			UIDRange(uint32_t first, uint32_t last);

			uint32_t getCount() const;

			uint32_t appendOne();

			bool isInRange(uint32_t value) const;
		};

		std::vector<UIDRange> givenUIDs;

		void removeUIDFrom(uint32_t index, uint32_t uid);

	public:
		UIDManager();

		/// Vergibt eine neue eindeutige UID
		uint32_t getNewUID();

		/// Gibt eine UID wieder frei, damit kann diese neu vergeben werden
		void releaseUID(uint32_t uid);

		/// Gibt alle UID's wieder frei
		void releaseAll();

		/// Prüft ob diese UID vergeben wurde
		bool isUIDGiven(uint32_t uid) const;

		/// Gibt zurück wie viele UID's vergeben wurden
		uint32_t getGivenUIDCount() const;

		/// Gibt zurück wie viele lücken zwischen den vergebenen UID's existieren
		uint32_t getUIDGapCount() const;

};
