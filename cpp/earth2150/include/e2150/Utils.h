#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include <vector>
#include <list>
#include <stdint.h>

#include "e2150/Map.h"
#include "e2150/MapPosition.h"

class Utils {
	public:
		template<typename T>
		static std::list<T> vectorToList(const std::vector<T>& v) {
			std::list<T> list(v.begin(), v.end());

			return list;
		}

		static std::list<uint32_t> mapPositionToPosition(const Map& map, std::list<MapPosition>& list) {
			std::list<uint32_t> outputList;

			for (std::list<MapPosition>::iterator i = list.begin(); i != list.end(); ++i) {
				 outputList.push_back(map.position(i->getX(), i->getY()));
			}
			return outputList;
		}

};


#endif // UTILS_H_INCLUDED
