#ifndef JPS_NAVIGATOR_H_
#define JPS_NAVIGATOR_H_

#include "e2150/Navigator.h"

class Map;
namespace ff {
	class JumpPointSearch;
}

class JPSNavigator : public Navigator {
	private:
		JPSNavigator(const JPSNavigator&);
		JPSNavigator& operator=(const JPSNavigator&);
		
		ff::JumpPointSearch* jps;
		
	public:
		JPSNavigator(const Map& map);
		~JPSNavigator();
		
		bool getPath(uint32_t start_index, uint32_t goal_index,
                     std::list<uint32_t>& path_list) const;
};

#endif // JPS_NAVIGATOR_H_
