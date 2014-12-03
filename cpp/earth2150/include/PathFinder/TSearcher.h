#ifndef TSEARCHER_H_INCLUDED
#define TSEARCHER_H_INCLUDED

#include <list>
#include <stddef.h>
#include <stdint.h>

class Map;

/**
* Breitensuchen Suchalgorithmus
* Identische Implementierung wie z.Z. im BlitzBasic Client
*/
class TSearcher {
	private:
		TSearcher(const TSearcher&);
		TSearcher& operator=(const TSearcher&);

		/// Speichert die Rohdatengröße
		const size_t mapSize;

		/// Bitmap für die Suche
		uint8_t* searchMap;

		const Map& map;

		/// Suchkarte mit 0-en überschreiben
		void clearSearchMap();

	public:
		TSearcher(const Map& map);

		~TSearcher();

		/// Such-funktion
		bool FindWay(uint32_t position1, uint32_t position2, std::list<uint32_t>& path_list);
};

#endif // TSEARCHER_H_INCLUDED
