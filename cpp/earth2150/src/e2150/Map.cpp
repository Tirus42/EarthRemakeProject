#include "e2150/Map.h"

#include "e2150/AStar.h"
#include "e2150/JPSNavigator.h"
#include "e2150/MovingUnit.h"
#include "tf/file.h"
#include "tf/time.h"
#include "e2150/Utils.h"

#include <cassert>
#include <iostream>
#include <fstream>

Map::Map(uint16_t width, uint16_t height) :
		width(width),
		height(height),
		borderWidth(1),
		heightMap(new uint16_t[width * height]),
		movementMap(new uint8_t[width * height]),
		statusMap(new uint8_t[width * height]),
		//navigator(new AStar(*this)),
		navigator(new JPSNavigator(*this)),		units(),		movingUnits(),
		spawnPositions(),
		viewerManager() {
}

Map::~Map() {
	delete navigator;
	delete[] statusMap;
	delete[] movementMap;
	delete[] heightMap;
}

size_t Map::getNeighbours(uint32_t position, uint32_t *neighbours) const {
	size_t numberOfNeighbours = 0;
	uint8_t directions = getDirections(position);
	if (directions & NORTH)      {neighbours[numberOfNeighbours++] = addNorth(position);}
	if (directions & NORTH_EAST) {neighbours[numberOfNeighbours++] = addNorthEast(position);}
	if (directions & EAST)       {neighbours[numberOfNeighbours++] = addEast(position);}
	if (directions & SOUTH_EAST) {neighbours[numberOfNeighbours++] = addSouthEast(position);}
	if (directions & SOUTH)      {neighbours[numberOfNeighbours++] = addSouth(position);}
	if (directions & SOUTH_WEST) {neighbours[numberOfNeighbours++] = addSouthWest(position);}
	if (directions & WEST)       {neighbours[numberOfNeighbours++] = addWest(position);}
	if (directions & NORTH_WEST) {neighbours[numberOfNeighbours++] = addNorthWest(position);}
	return numberOfNeighbours;
}

uint8_t Map::getDirection(uint32_t start_index, uint32_t goal_index) const {
	int16_t x = positionX(goal_index) - positionX(start_index);
	int16_t y = positionY(goal_index) - positionY(start_index);
	bool x0 = x < 0;
	bool y0 = y < 0;
	int16_t x2 = 2 * x;
	int16_t y2 = 2 * y;
	if (x0) {
		if (x <= y2 && y2 <= -x) {
			return Map::DIRECTION_WEST;
		}
	} else {
		if (-x <= y2 && y2 <= x) {
			return Map::DIRECTION_EAST;
		}
	}
	if (y0) {
		if (y <= x2 && x2 <= -y) {
			return Map::DIRECTION_NORTH;
		}
	} else {
		if (-y <= x2 && x2 <= y) {
			return Map::DIRECTION_SOUTH;
		}
	}
	if (x0) {
		if (y0) {
			return Map::DIRECTION_NORTH_WEST;
		}
		return Map::DIRECTION_SOUTH_WEST;
	}
	if (y0) {
		return Map::DIRECTION_NORTH_EAST;
	}
	return Map::DIRECTION_SOUTH_EAST;
}

bool Map::getWay(uint32_t start_index, uint32_t goal_index, std::list<uint32_t>& path_list) const {
	assert(fieldOnMap(start_index));
	assert(fieldOnMap(goal_index));

	return navigator->getPath(start_index, goal_index, path_list);
}

void Map::setFieldStatusFlag(uint32_t position, uint8_t statusFlag, bool value) {
	assert(fieldOnMap(position));

	if (value) {
		statusMap[position] |= statusFlag;
	} else {
		statusMap[position] &= ~statusFlag;
	}
}

bool Map::getFieldStatusFlag(uint32_t position, uint8_t statusFlag) const {
	assert(fieldOnMap(position));

	return statusMap[position] & statusFlag;
}

bool Map::addUnit(Unit& unit, uint16_t x, uint16_t y) {
	assert(fieldOnMap(x, y));

	uint32_t pos = position(x, y);

	//Prüfen ob das Feld frei ist um eine Einheit darauf zu setzen
	if (!isFieldFree(pos)) {
		return false;
	}

	//Einheit auf Spielfeld setzen, und in Liste der Einheiten eintragen
	units.insert(std::pair<uint32_t, Unit*>(unit.getID(), &unit));

	//Und deren Position setzen
	unit.setPosition(x, y);

	//Auf der Karte verzeichnen, dass hier nun eine Einheit steht
	setFieldStatusFlag(position(x, y), STATUS_UNIT, true);

	//Spieler über neue Einheit informieren
	viewerManager.createEntity(unit);

	std::cout << "Neue Einheit (ID: " << unit.getID() << ") auf die Karte gesetzt ("
	<< x << ":" << y << ")" << std::endl;

	return true;
}

void Map::removeUnit(Unit& unit) {
	//Todo: Moving Unit Eintrag entfernen
	//Todo: Einheit aus Liste entfernen

	viewerManager.removeEntity(unit);
}

void Map::UnitDriveTo(Unit& unit, uint32_t target) {
	assert(fieldOnMap(target));

	std::list<uint32_t> way;

	uint32_t startPos = position(unit.getX(), unit.getY());
	if (getWay(startPos, target, way)) {

		unit.setWay(way);

		uint32_t position = unit.getNextWaypoint();

		MovingUnit* m = new MovingUnit(unit, Utils::getAngle(*this, startPos, position), MilliSecs(), *this);
		movingUnits.insert(m);

		std::cout << "Einheit startet Bewegung!\n";
	}
}

void Map::addSpawnPoint(const MapPosition& position, const Faction* faction) {
	assert(fieldOnMap(position));

	spawnPositions.push_back(position);

	//Todo: Faction verwalten
}
void Map::updateMovementMap() {
	updateMovementMap(0, 0, width, height);
	return;

	updateMovementMap(borderWidth, borderWidth, width - borderWidth, height - borderWidth);
}

void Map::updateMovementMap(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
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
				if (passables[addNorth(index)]) {
					value |= NORTH;
				}
				if (passables[addNorthEast(index)]) {
					value |= NORTH_EAST;
				}
				if (passables[addEast(index)]) {
					value |= EAST;
				}
				if (passables[addSouthEast(index)]) {
					value |= SOUTH_EAST;
				}
				if (passables[addSouth(index)]) {
					value |= SOUTH;
				}
				if (passables[addSouthWest(index)]) {
					value |= SOUTH_WEST;
				}
				if (passables[addWest(index)]) {
					value |= WEST;
				}
				if (passables[addNorthWest(index)]) {
					value |= NORTH_WEST;
				}
			}
			movementMap[index] = value;
		}
	}
	delete[] passables;
}

void Map::updateMovementMap(uint32_t position1, uint32_t position2) {
	uint16_t x1 = positionX(position1);
	uint16_t y1 = positionY(position1);
	uint16_t x2 = positionX(position2);
	uint16_t y2 = positionY(position2);
	updateMovementMap(x1, y1, x2, y2);
}

void Map::updateMovementMapWithBorder() {
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
			if ((index > width-1) && (passables[addNorth(index)])) {
				value |= NORTH;
			}
			if (passables[addSouth(index)]) {
				value |= SOUTH;
			}
			if ((index % width != width-2) && (passables[addEast(index)])) {
				value |= EAST;
			}
			if ((index % width > 0) && (passables[addWest(index)])) {
				value |= WEST;
			}
			if (((index % width != width-2) && (index > width-1)) && (passables[addNorthEast(index)])) {
				value |= NORTH_EAST;
			}
			if ((index % width != width-2) && (passables[addSouthEast(index)])) {
				value |= SOUTH_EAST;
			}
			if ((index % width > 0) && (passables[addSouthWest(index)])) {
				value |= SOUTH_WEST;
			}
			if (((index % width > 0) && (index > width)) && (passables[addNorthWest(index)])) {
				value |= NORTH_WEST;
			}
		}
		movementMap[index] = value;
	}
	delete[] passables;
}

bool Map::loadHeightMapRAW(const std::string& filename) {
	uint32_t fileSize = FileSize(filename);
	uint32_t dataSize = width * height;

	if (fileSize > dataSize * 2) {
		fileSize = dataSize * 2;  //Wir arbeiten mit Shorts (2 Byte)

		std::cout << "Warnung: Map Datei ist groeser als erwartet, lese nur " << fileSize << " bytes ein!\n";
	}

	std::ifstream file(filename.c_str(), std::ifstream::in | std::ifstream::binary);

	if(!file.good()) {
		std::cout << "Konnte Map-Datei nicht lesen! (" << filename << ")\n";
		return false;
	}

	//Daten einlesen
	file.read((char*)heightMap, fileSize);

	//Datei wieder schließen
	file.close();

	return true;
}

uint16_t Map::getHeightDiffOnField(uint32_t position) const {
	uint16_t h1 = heightMap[position];
	uint16_t h2 = heightMap[addEast(position)];
	uint16_t h3 = heightMap[addSouth(position)];
	uint16_t h4 = heightMap[addSouthEast(position)];

	uint16_t maxValue = std::max(std::max(h1, h2), std::max(h3, h4));
	uint16_t minValue = std::min(std::min(h1, h2), std::min(h3, h4));

	return maxValue - minValue;
}

bool Map::isFieldFree(uint32_t position) const {
	// Todo: Auf Gebäude und andere Objekte prüfen
	return !getFieldStatusFlag(position, STATUS_UNIT);
}

void Map::updateGameField(uint32_t currentTime) {

}
