#include "e2150/JPSNavigator.h"

#include "ff/JumpPointSearch.h"

JPSNavigator::JPSNavigator(const Map& map) :
	INavigator(map),
	jps(new ff::JumpPointSearch(map)) {}

JPSNavigator::~JPSNavigator() {
	delete jps;
}

bool JPSNavigator::getPath(uint32_t start_index, uint32_t goal_index,
			 std::list<uint32_t>& path_list) const {
	return jps->Solve(start_index, goal_index, path_list);
}
