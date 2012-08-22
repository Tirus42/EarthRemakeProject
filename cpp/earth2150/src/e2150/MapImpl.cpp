#include "e2150/MapImpl.h"

#include "e2150/AStar.h"
#include "tf/file.h"
#include <fstream>
#include <iostream>

MapImpl::MapImpl(uint16_t width, uint16_t height) :
		Map(width, height),
		heightMap(width * height),
		movementMap(width * height),
		statusMap(width * height),
		borderWidth(1),
		navigator(new AStar()),
		units(),
		spawnPositions(),
		viewerManager() {
}

MapImpl::~MapImpl() {
	delete navigator;
}

void MapImpl::updateMovementMap() {
	updateMovementMap(0, 0, width, height);
	return;

	updateMovementMap(borderWidth, borderWidth, width - borderWidth, height - borderWidth);
}

void MapImpl::updateMovementMap(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
	if (x1 > x2) {
		std::swap(x1, x2);
	}
	if (y1 > y2) {
		std::swap(y1, y2);
	}

	uint16_t w = x2 - x1;
	uint16_t h = y2 - y1;
	bool* passables = new bool[w * h];

	//Felder definieren, welche nicht "zu schief" sind
	for (uint16_t y = y1; y < y2; ++y) {
		for (uint16_t x = x1; x < x2; ++x) {
			uint32_t index = position(x, y);
			passables[index] = (getHeightDiffOnField(index) < MAX_HEIGHTDIFF);
		}
	}

	//Feld um eines "Einengen"
	++x1;
	--x2;
	++y1;
	--y2;

	//Nun Komplette Map durchgehen, und in prüfen, welche Nachbarfelder auch begehbar sind
	for (uint16_t y = y1; y < y2; ++y) {
		for (uint16_t x = x1; x < x2; ++x) {
			uint32_t index = position(x, y);
			uint8_t value = 0;

			//Ist dieses Feld begehbar?
			if (passables[index]) {
				if (passables[index - width]) {
					value |= NORTH;
				}
				if (passables[index + width]) {
					value |= SOUTH;
				}
				if (passables[index + 1]) {
					value |= EAST;
				}
				if (passables[index - 1]) {
					value |= WEST;
				}
				if (passables[index + 1 - width]) {
					value |= NORTH_EAST;
				}
				if (passables[index + 1 + width]) {
					value |= SOUTH_EAST;
				}
				if (passables[index - 1 + width]) {
					value |= SOUTH_WEST;
				}
				if (passables[index - 1 - width]) {
					value |= NORTH_WEST;
				}
			}
			movementMap[index] = value;
		}
	}
	delete[] passables;
}

void MapImpl::updateMovementMap(uint32_t position1, uint32_t position2) {
	uint16_t x1 = positionX(position1);
	uint16_t y1 = positionY(position1);
	uint16_t x2 = positionX(position2);
	uint16_t y2 = positionY(position2);
	updateMovementMap(x1, y1, x2, y2);
}

void MapImpl::updateMovementMapWithBorder() {
	bool* passables = new bool[width * height];

	//Felder definieren, welche nicht "zu schief" sind
	uint32_t size = (width-1) * (height-1);

	for (uint32_t i = 0; i < size; ++i) {
		passables[i] = (getHeightDiffOnField(i) < MAX_HEIGHTDIFF);
	}

	//Nun Komplette Map durchgehen, und in prüfen, welche Nachbarfelder auch begehbar sind
	for (int32_t index = 0; index < width*height-1; ++index) {
		uint8_t value = 0;

		//Ist dieses Feld begehbar?
		if (passables[index]) {
			if ((index > width-1) && (passables[index - width])) {
				value |= NORTH;
			}
			if (passables[index + width]) {
				value |= SOUTH;
			}
			if ((index % width != width-2) && (passables[index + 1])) {
				value |= EAST;
			}
			if ((index % width > 0) && (passables[index - 1])) {
				value |= WEST;
			}
			if (((index % width != width-2) && (index > width-1)) &&(passables[index + 1 - width])) {
				value |= NORTH_EAST;
			}
			if ((index % width != width-2) && (passables[index + 1 + width])) {
				value |= SOUTH_EAST;
			}
			if ((index % width > 0) && (passables[index - 1 + width])) {
				value |= SOUTH_WEST;
			}
			if (((index % width > 0) && (index > width)) && (passables[index - 1 - width])) {
				value |= NORTH_WEST;
			}
		}
		movementMap[index] = value;
	}
	delete[] passables;
}

uint32_t MapImpl::getNumberOfMoveableFields() const {
	uint32_t moveableFields = 0;
	for (uint16_t y = 0; y < height; y++) {
		for (uint16_t x = 0; x < width; x++) {
			uint32_t index = position(x, y);
			if (movementMap[index]) {
				moveableFields++;
			}
		}
	}
	return moveableFields;
}

uint16_t MapImpl::getHeightDiffOnField(uint32_t position) const {
	uint16_t h1 = heightMap[position];
	uint16_t h2 = heightMap[position + 1];
	uint16_t h3 = heightMap[position + width];
	uint16_t h4 = heightMap[position + width + 1];

	uint16_t maxValue = std::max(std::max(h1, h2), std::max(h3, h4));
	uint16_t minValue = std::min(std::min(h1, h2), std::min(h3, h4));

	return maxValue - minValue;
}

std::vector<MapPosition> MapImpl::getNeighbourPositions(uint16_t x, uint16_t y) const {
	MapPosition _[8];
	uint16_t x0 = x - 1, y0 = y - 1, x1 = x + 1, y1 = y + 1;
	size_t size = 0;
	uint8_t directions=getDirections(x, y);
	if (directions&NORTH)      {_[size++] = MapPosition(x,  y0);}
	if (directions&NORTH_EAST) {_[size++] = MapPosition(x1, y0);}
	if (directions&EAST)       {_[size++] = MapPosition(x1, y );}
	if (directions&SOUTH_EAST) {_[size++] = MapPosition(x1, y1);}
	if (directions&SOUTH)      {_[size++] = MapPosition(x,  y1);}
	if (directions&SOUTH_WEST) {_[size++] = MapPosition(x0, y1);}
	if (directions&WEST)       {_[size++] = MapPosition(x0, y );}
	if (directions&NORTH_WEST) {_[size++] = MapPosition(x0, y0);}
	return std::vector<MapPosition>(_, _+size);
}

std::vector<MapPosition> MapImpl::getWay(const Unit& unit, uint32_t destination) const {
	return navigator->getPath(*this, unit, positionX(destination), positionY(destination));
}

bool MapImpl::loadHeightMapRAW(const std::string& filename) {
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

bool MapImpl::isFieldFree(uint32_t position) const {
	// Todo: Auf Gebäude und andere Objekte prüfen
	return !getFieldStatusFlag(position, STATUS_UNIT);
}

void MapImpl::setFieldStatusFlag(uint32_t position, uint8_t statusFlag, bool value) {
	if (value) {
		statusMap[position] |= statusFlag;
	} else {
		statusMap[position] &= ~statusFlag;
	}
}

bool MapImpl::getFieldStatusFlag(uint32_t position, uint8_t statusFlag) const {
	return statusMap[position] & statusFlag;
};

bool MapImpl::addUnit(Unit& unit, uint16_t x, uint16_t y) {
	uint32_t pos = position(x, y);

	//Prüfen ob das Feld frei ist um eine Einheit darauf zu setzen
	if (!isFieldFree(pos)) {
		return false;
	}

	//Einheit auf Spielfeld setzen, und in Liste der Einheiten eintragen
	units.insert(std::pair<uint32_t, Unit*>(unit.getID(), &unit));

	//Und deren Position setzen
	unit.setPosition(x, y);

	//Spieler über neue Einheit informieren
	viewerManager.createEntity(unit);

	std::cout << "Neue Einheit auf die Map gesetzt\n";

	return true;
}

void MapImpl::removeUnit(Unit& unit) {
	//Todo: Einheit aus Liste entfernen

	viewerManager.removeEntity(unit);
}

void MapImpl::addSpawnPoint(const MapPosition& position, const Faction* faction) {
	spawnPositions.push_back(position);

	//Todo: Faction verwalten
}
