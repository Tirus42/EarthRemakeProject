#include "Map/HeightMap.h"

#include <algorithm>

#include <memory.h>
#include <cassert>

HeightMap::HeightMap(uint16_t mapWidth, uint16_t mapHeight) :
	heightData(0),
	mapWidth(mapWidth),
	mapHeight(mapHeight) {

	heightData = allocateMemory(mapWidth, mapHeight);

	assert(heightData);
}

HeightMap::~HeightMap() {
	freeMemory(heightData);
}

void HeightMap::resize(uint16_t newWidth, uint16_t newHeight) {
	uint16_t* newPtr = allocateMemory(newWidth, newHeight);

	assert(newPtr);

	uint16_t copyWidth = std::min(mapWidth, newWidth);
	uint16_t copyHeight = std::min(mapHeight, newHeight);

	for (uint32_t y = 0; y < copyHeight; ++y) {
		memcpy(newPtr + y * newWidth, heightData + y * mapWidth, copyWidth * sizeof(uint16_t));
	}

	freeMemory(heightData);

	heightData = newPtr;
	mapWidth = newWidth;
	mapHeight = newHeight;
}

const uint16_t& HeightMap::operator[](uint32_t index) const {
	assert(index < mapWidth * mapHeight);
	return heightData[index];
}

uint16_t& HeightMap::operator[](uint32_t index) {
	assert(index < mapWidth * mapHeight);
	return heightData[index];
}

const uint16_t& HeightMap::operator()(uint16_t x, uint16_t y) const {
	assert(x < mapWidth);
	assert(y < mapHeight);

	return heightData[y * mapWidth + x];
}

uint16_t& HeightMap::operator()(uint16_t x, uint16_t y) {
	assert(x < mapWidth);
	assert(y < mapHeight);

	return heightData[y * mapWidth + x];
}

uint16_t* HeightMap::allocateMemory(uint16_t mapWidth, uint16_t mapHeight) const {
    uint16_t* ptr = new uint16_t[mapWidth * mapHeight];
    memset(ptr, 0, mapWidth * mapHeight);
    return ptr;
}

void HeightMap::freeMemory(uint16_t* ptr) const {
	delete[] ptr;
}
