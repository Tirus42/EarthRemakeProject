#ifndef TIRUSSEARCH_H_INCLUDED
#define TIRUSSEARCH_H_INCLUDED

#include "PathFinder/INavigator.h"

class TSearcher;

/**
* Tirus-Breitensuchen Klasse
* Identische Implementierung wie in BlitzBasic
*/
class TNavigator : public INavigator {
	private:
		TNavigator(const TNavigator&);
		TNavigator& operator=(const TNavigator&);

		TSearcher* searcher;

	public:
		TNavigator(const Map& map);
		~TNavigator();

		bool getPath(uint32_t start_index, uint32_t goal_index,
                     std::list<uint32_t>& path_list) const;
};

#endif // TIRUSSEARCH_H_INCLUDED
