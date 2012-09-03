#ifndef VISUALMAP_H_INCLUDED
#define VISUALMAP_H_INCLUDED

#include "e2150/Map.h"

#include <vector>

class VisualMapPart;

class VisualMap : public Map {
	private:
		std::vector<VisualMapPart*> mapParts;

	public:
		VisualMap(uint16_t width, uint16_t height);


};

#endif // VISUALMAP_H_INCLUDED
