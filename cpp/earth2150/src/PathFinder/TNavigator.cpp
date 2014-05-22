#include "PathFinder/TNavigator.h"

#include "PathFinder/TSearcher.h"

TNavigator::TNavigator(const Map& map) :
	INavigator(map),
	searcher(new TSearcher(map)) {

}

TNavigator::~TNavigator() {
	delete searcher;
}


bool TNavigator::getPath(uint32_t start_index, uint32_t goal_index, std::list<uint32_t>& path_list) const {
	return searcher->FindWay(start_index, goal_index, path_list);
}
