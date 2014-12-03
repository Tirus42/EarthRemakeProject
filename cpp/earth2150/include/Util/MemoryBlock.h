#ifndef MEMORYBLOCK_H_INCLUDED
#define MEMORYBLOCK_H_INCLUDED

#include <vector>

#include <stdint.h>
#include <stddef.h>
#include <cassert>

/**
* Struktur ähnlich einem std::array das den angegebenen Speicherplatz
* direkt komplett belegt.
* Einzelne Elemente können eingefügt und wieder entfernt werden.
* Beim einfügen wird der Index zurück gegeben.
* Die Struktur führt eine Liste über gesetzte Elemente und sichert so,
* das ein Slot nicht doppelt beschrieben wird.
* Sollte die Struktur vollständig gefüllt sein, können keine weiteren Elemente
* eingefügt werden, bis Elemente entfernt oder die Struktur vergrößert wurde.
*/
template <class T>
class MemoryBlock {
	private:
		/// Speicher für die Elemente
		std::vector<T> entry;

		/// Speicher für die Flags, ob das Element gesetzt wurde/gültig ist
		std::vector<bool> entryUse;

	public:
		MemoryBlock(size_t maxElements) :
			entry(),
			entryUse() {

			resize(maxElements);
		}

		~MemoryBlock() {}

		/// Bestimmt eine neue Größe.
		/// Vorhandene Elemente werden nicht entfernt.
		void resize(size_t newSize) {
			entry.resize(newSize);
			entryUse.resize(newSize, false);
		}

		/// Gibt die aktuelle Maximale Anzahl an Elementen zurück.
		size_t getSize() const {
			return entry.size();
		}

        /// Gibt das angegebene Element zurück.
        /// Der Index muss gültig sein.
		const T& getEntry(size_t pos) const {
			assert(pos < entry.size());
			assert(entryUse[pos] == true);

			return entry.at(pos);
		}

		/// Gibt das angegebene Element zurück.
        /// Der Index muss gültig sein.
		T& getEntry(size_t pos) {
			assert(pos < entry.size());
			assert(entryUse[pos] == true);

			return entry[pos];
		}

		/// Setzt das Element an die angegebene Position.
		void setEntry(size_t pos, const T& element) {
			assert(pos < entry.size());

			entry[pos] = element;
			entryUse[pos] = true;
		}

		/// Gibt zurück, ob ein Element an der Position gesetzt ist.
		bool isEntrySet(size_t pos) const {
			assert(pos < entry.size());

			return entryUse[pos];
		}

		/// Findet eine freie Speicherstelle.
		/// \return Die gefundene Stelle, < 0 wenn kein Platz mehr vorhanden ist.
		ptrdiff_t findFreeSpace() const {
			// Todo: Durch Merken der letzten Position optimieren
			size_t i = 0;

			for (; i < entry.size(); ++i) {
				if (!isEntrySet(i))
					break;
			}

			if (i < entry.size())
				return i;

			return (-1);
		}

		/// Fügt ein Element an eine freie Stelle ein.
		/// \return Die Stelle an welche das Element eingefügt wurde, < 0 wenn kein Platz mehr vorhanden war.
		ptrdiff_t addEntry(const T& element) {
			ptrdiff_t pos = findFreeSpace();

			if (pos < 0)
				return pos;

			entry[pos] = element;
			entryUse[pos] = true;

			return pos;
		}

		/// Entfernt ein Element an der angegebenen Position.
		/// Es muss bisher ein Element an der Stelle vorhanden gewesen sein.
		void removeEntry(size_t pos) {
			assert(pos < entry.size());
			assert(entryUse[pos] == true);

			entryUse[pos] = false;
		}

		/// Entfernt alle Elemente.
		/// (Es werden nur die Gültigkeitseinträge entfernt, das Element selbst wird behalten
		/// bis es überschrieben wird)
		void clear() {
			for (size_t i = 0; i < entryUse.size(); ++i)
				entryUse[i] = false;
		}
};


#endif // MEMORYBLOCK_H_INCLUDED
