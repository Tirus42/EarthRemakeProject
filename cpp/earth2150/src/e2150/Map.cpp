#include "e2150/Map.h"

#include "e2150/AStar.h"
#include <fstream>
#include <iostream>

Map::Map(uint16_t width, uint16_t height) :
		width(width),
		height(height),
		heightMap(width * height),
		movementMap(width * height),
		borderWidth(1),
		navigator(new AStar()) {
}

Map::~Map() {
}

void Map::updateMovementMap() {
	updateMovementMap(0, 0, width, height);
	return;

	updateMovementMap(borderWidth, borderWidth, width - borderWidth, height - borderWidth);
}

void Map::updateMovementMap(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
	if (x1 > x2) {
		return updateMovementMap(x2, y1, x1, y2);
	}
	if (y1 > y2) {
		return updateMovementMap(x1, y2, x2, y1);
	}
	int32_t w = x2 - x1;
	int32_t h = y2 - y1;
	bool* buffer = new bool[w * h];

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
	for (uint32_t y = y1; y < y2; ++y) {
		for (uint32_t x = x1; x < x2; ++x) {
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
	uint16_t x1 = positionX(position1);
	uint16_t y1 = positionY(position1);
	uint16_t x2 = positionX(position2);
	uint16_t y2 = positionY(position2);

	updateMovementMap(x1, y1, x2, y2);
}

void Map::updateMovementMapWithBorder() {
	bool* buffer = new bool[width * height];

	//Felder definieren, welche nicht "zu schief" sind
	uint32_t size = (width-1) * (height-1);

	for (uint32_t i = 0; i < size; ++i) {
		buffer[i] = (getHeightDiffOnField(i) < MAX_HEIGHTDIFF);
	}

	//Nun Komplette Map durchgehen, und in prüfen, welche Nachbarfelder auch begehbar sind
	for (uint32_t index = 0; index < width*height-1; ++index) {
		uint8_t value = 0;

		//Ist dieses Feld begehbar?
		if (buffer[index]) {
			if ((index > width-1) && (buffer[index - width])) {
				value |= NORTH;
			}
			if (buffer[index + width]) {
				value |= SOUTH;
			}
			if ((index % width != width-2) && (buffer[index + 1])) {
				value |= EAST;
			}
			if ((index % width > 0) && (buffer[index - 1])) {
				value |= WEST;
			}
			if (((index % width != width-2) && (index > width-1)) &&(buffer[index + 1 - width])) {
				value |= NORTH_EAST;
			}
			if ((index % width != width-2) && (buffer[index + 1 + width])) {
				value |= SOUTH_EAST;
			}
			if ((index % width > 0) && (buffer[index - 1 + width])) {
				value |= SOUTH_WEST;
			}
			if (((index % width > 0) && (index > width)) && (buffer[index - 1 - width])) {
				value |= NORTH_WEST;
			}
		}
		movementMap[index] = value;
	}
	delete[] buffer;
}

uint32_t Map::getNumberOfMoveableFields() const {
	uint32_t moveableFields = 0;
	for (uint32_t y=0; y<height; ++y) {
		for (uint32_t x=0; x<width; ++x) {
			uint32_t index = position(x, y);
			if (movementMap[index]) {
				moveableFields++;
			}
		}
	}
	return moveableFields;
}

uint16_t Map::getHeightDiffOnField(uint32_t position) const {
	uint16_t h1 = heightMap[position];
	uint16_t h2 = heightMap[position + 1];
	uint16_t h3 = heightMap[position + width];
	uint16_t h4 = heightMap[position + width + 1];

	uint16_t maxValue = std::max(std::max(h1, h2), std::max(h3, h4));
	uint16_t minValue = std::min(std::min(h1, h2), std::min(h3, h4));

	return maxValue - minValue;
}

std::vector<MapPosition> Map::getNeighbourPositions(uint32_t x, uint32_t y) const{
	std::vector<MapPosition> _(8);
	int x0 = x - 1, y0 = y - 1, x1 = x + 1, y1 = y + 1;
	uint32_t i = 0;
	const uint8_t DIRECTIONS=getDirections(x, y);
	if (DIRECTIONS&NORTH) {_[i++] = MapPosition(x, y0);}
	if (DIRECTIONS&NORTH_EAST) {_[i++] = MapPosition(x1, y0);}
	if (DIRECTIONS&EAST) {_[i++] = MapPosition(x1, y);}
	if (DIRECTIONS&SOUTH_EAST) {_[i++] = MapPosition(x1, y1);}
	if (DIRECTIONS&SOUTH) {_[i++] = MapPosition(x, y1);}
	if (DIRECTIONS&SOUTH_WEST) {_[i++] = MapPosition(x0, y1);}
	if (DIRECTIONS&WEST) {_[i++] = MapPosition(x0, y);}
	if (DIRECTIONS&NORTH_WEST) {_[i++] = MapPosition(x0, y0);}
	return _;
}

std::vector<MapPosition> Map::getWay(const Unit& unit, uint32_t destination) const{
	return navigator->getPath(*this, unit, positionX(destination), positionY(destination));
}

bool Map::loadHeightMapRAW(const std::string& filename) {
	uint32_t fileSize = FileSize(filename);
	uint32_t dataSize = width * height;

	if (fileSize > uint32_t(width * height)*2) {
		fileSize = width * height * 2;  //Wir arbeiten mit Shorts (2 Byte)

		std::cout << "Warnung: Map Datei ist groeser als erwartet, lese nur " << fileSize << " bytes ein!\n";
	}

	std::ifstream file(filename.c_str(), std::ifstream::in | std::ifstream::binary);

	if(!file.good()) {
		std::cout << "Konnte Map-Datei nicht lesen! (" << filename << ")\n";
		return false;
	}

	//Themporären Speicher anlegen (Todo: Direkt in Heightmap laden)
	uint16_t* data = new uint16_t[dataSize];

	//Daten einlesen
	file.read((char*)data, fileSize);

	//Datei wieder schließen
	file.close();

	//Nun in die Heightmap schreiben (...)
	for (uint32_t i = 0; i < dataSize; ++i) {
		heightMap[i] = data[i];
		//std::cout << heightMap[i];
	}

	//Speicher wieder freigeben (...)
	delete[] data;

	return true;
}
