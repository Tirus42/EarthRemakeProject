#ifndef NAVIGATOR_H
#define NAVIGATOR_H

#include "Map/Map.h"
#include "e2150/Unit.h"

#include <list>

class Map;

class Navigator {
	protected:
		const Map& map;

	public:
		Navigator(const Map& map) : map(map) {}
		virtual ~Navigator() {}

		/// Generiert den Pfad zwischen den angegebenen Punkten und schreibt ihn
		/// in die Liste, gibt true zurück, wenn der Pfad erfolgreich gefunden
		/// und generiert wurde.
		virtual bool getPath(uint32_t start_index, uint32_t goal_index,
                             std::list<uint32_t>& path_list) const = 0;

		/// Wird aufgerufen, wenn sich Bereiche der Höhenkarte in der
		/// Map ändern.
		virtual void mapDataChanged(uint32_t top_left, uint16_t width, uint16_t height) {}
};

#endif // NAVIGATOR_H
