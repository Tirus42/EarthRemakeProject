#include "Map/MapBitLayer.h"

#include <cstring>
#include <stdint.h>

MapBitLayer::MapBitLayer(uint16_t minX, uint16_t minY, uint16_t width, uint16_t height):
		minX(minX),
		minY(minY),
		width(width),
		height(height),
		bits(new uint32_t[(width*height+31) >> 5]){
	::memset(bits, 0, ((width*height+31) >> 5) << 2);
}

MapBitLayer::~MapBitLayer(){
	delete[] bits;
}
