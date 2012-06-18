#include "Map.h"

Map::Map(uint32_t width, uint32_t height) :
		width(width),
		height(height),
		heightMap(width * height),
		movementMap(width * height),
		borderWidth(1) {
    updateMovementMap();
}

Map::~Map() {
}

void Map::updateMovementMap() {
    updateMovementMap(borderWidth, borderWidth, width - borderWidth, height - borderWidth);
}

void Map::updateMovementMap(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
    if(x1 > x2) {
		return updateMovementMap(x2, y1, x1, y2);
    }
    if(y1 > y2) {
		return updateMovementMap(x1, y2, x2, y1);
    }
    int32_t w = x2 - x1;
    int32_t h = y2 - y1;
    bool* buffer = new bool[w * h]; //bool* buffer = new bool[w][h]

    //Felder definieren, welche nicht "zu schief" sind
    for (uint32_t y = y1; y < y2; ++y) {
        for (uint32_t x = x1; x < x2; ++x) {
            uint32_t index = position(x, y);
            buffer[index] = (getHeightDiffOnField(index) < MAX_HEIGHTDIFF);
        }
    }

    //Feld um eines "Einengen"
    ++x1;
    --x2;
    ++y1;
    --y2;

    //Nun Komplette Map durchgehen, und in prüfen, welche Nachbarfelder auch begehbar sind
    for (uint32_t y = y1; y<y2; ++y) {
        for (uint32_t x = x1; x<x2; ++x) {
            uint32_t index = position(x, y);
            uint8_t value = 0;

            //Ist dieses Feld begehbar?
            if (buffer[index]) {
                if (buffer[index - width]) {
                    value |= NORTH;
                }
                if (buffer[index + width]) {
                    value |= SOUTH;
                }
                if (buffer[index + 1]) {
                    value |= EAST;
                }
                if (buffer[index - 1]) {
                    value |= WEST;
                }
                if (buffer[index + 1 - width]) {
                    value |= NORTH_EAST;
                }
                if (buffer[index + 1 + width]) {
                    value |= SOUTH_EAST;
                }
                if (buffer[index - 1 + width]) {
                    value |= SOUTH_WEST;
                }
                if (buffer[index - 1 - width]) {
                    value |= NORTH_WEST;
                }
            }
			movementMap[index] = value;
        }
    }
    delete[] buffer;
}

void Map::updateMovementMap(uint32_t position1, uint32_t position2) {
	uint16_t x1=positionX(position1);
	uint16_t y1=positionY(position1);
	uint16_t x2=positionX(position2);
	uint16_t y2=positionY(position2);
	updateMovementMap(x1, y1, x2, y2);
}

uint32_t Map::getNumberOfMoveableFields() const{
	uint32_t moveableFields = 0;
    for (uint32_t y=0; y<height; ++y) {
        for (uint32_t x=0; x<width; ++x) {
			uint32_t index = position(x, y);
			if(movementMap[index]) {
				moveableFields++;
			}
        }
    }
    return moveableFields;
}

uint32_t Map::getHeightDiffOnField(uint32_t position) const{
    uint16_t h1 = heightMap[position];
    uint16_t h2 = heightMap[position + 1];
    uint16_t h3 = heightMap[position + width];
    uint16_t h4 = heightMap[position + width + 1];

    uint16_t maxValue = std::max(std::max(h1, h2), std::max(h3, h4));
    uint16_t minValue = std::min(std::min(h1, h2), std::min(h3, h4));

    return maxValue - minValue;
}
