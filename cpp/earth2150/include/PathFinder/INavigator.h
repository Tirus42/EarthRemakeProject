#ifndef NAVIGATOR_H
#define NAVIGATOR_H

#include <stdint.h>
#include <list>

class Map;

class INavigator {
	protected:
		const Map& map;

	public:
		INavigator(const Map& map) : map(map) {}
		virtual ~INavigator() {}

		/// Generiert den Pfad zwischen den angegebenen Punkten und schreibt ihn
		/// in die Liste, gibt true zurück, wenn der Pfad erfolgreich gefunden
		/// und generiert wurde.
		virtual bool getPath(uint32_t start_index, uint32_t goal_index,
                             std::list<uint32_t>& path_list) const = 0;

		/// Wird aufgerufen, wenn sich Bereiche der Höhenkarte in der
		/// Map ändern.
		virtual void mapDataChanged(uint32_t top_left, uint16_t width, uint16_t height) {}
};

#endif

