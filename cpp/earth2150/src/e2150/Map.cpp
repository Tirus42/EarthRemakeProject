#include "e2150/Map.h"

#include "e2150/AStar.h"
#include "e2150/JPSNavigator.h"
#include "PathFinder/TNavigator.h"
#include "e2150/MovingUnit.h"
#include "tf/file.h"
#include "tf/time.h"
#include "e2150/Utils.h"
#include "e2150/HumanPlayer.h"

#include <cassert>
#include <cstring>
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
		//navigator(new JPSNavigator(*this)),
		navigator(new TNavigator(*this)),		units(),		movingUnits(),
		spawnPositions(),
		viewerManager(*this) {
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
		movingUnits.push(m);
		std::cout << "Einheit startet Bewegung!\n";
	}
}

void Map::addSpawnPoint(const MapPosition& position, const Faction* faction) {
	assert(fieldOnMap(position));

	spawnPositions.push_back(position);
	//TODO: Faction verwalten
}

void Map::updateMovementMap() {
	updateMovementMap(borderWidth, borderWidth, width - borderWidth, height - borderWidth);
}

void Map::updateMovementMap(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
	assert(x1 < x2);
	assert(y1 < y2);

	uint16_t width = x2 - x1;
	uint16_t height = y2 - y1;
	uint16_t width2 = width + 2;	uint16_t height2 = height + 2;
	bool *passables = new bool[width2 * height2];
	::memset(passables, true, width2 * height2 * sizeof(bool));

	//Felder definieren, welche nicht "zu schief" sind
	for (uint16_t y = 0; y != height2; y++) {
		for (uint16_t x = 0; x != width2; x++) {
			uint16_t realX = x1 - 1 + x;
			uint16_t realY = y1 - 1 + y;
			if (
				realX < borderWidth || realX >= this->width - borderWidth
				||
				realY < borderWidth || realY >= this->height - borderWidth
				||
				getHeightDiffOnField(position(realX, realY)) >= MAX_HEIGHTDIFF
			) {
				passables[y * width2 + x] = false;
			}
		}
	}

	//Nun Komplette Map durchgehen, und in prüfen, welche Nachbarfelder auch begehbar sind
	for (uint16_t y = 1; y <= height; y++) {
		for (uint16_t x = 1; x <= width; x++) {
			uint32_t position = y * width2 + x;
			uint8_t direction = 0x00;

			//Ist dieses Feld begehbar?
			if (passables[position]) {
				if (passables[position - width2]) {
					direction |= NORTH;
				}
				if (passables[position - width2 + 1]) {
					direction |= NORTH_EAST;
				}
				if (passables[position + 1]) {
					direction |= EAST;
				}
				if (passables[position + width2 + 1]) {
					direction |= SOUTH_EAST;
				}
				if (passables[position + width2]) {
					direction |= SOUTH;
				}
				if (passables[position + width2 - 1]) {
					direction |= SOUTH_WEST;
				}
				if (passables[position - 1]) {
					direction |= WEST;
				}
				if (passables[position - width2 - 1]) {
					direction |= NORTH_WEST;
				}
			}
			movementMap[this->position(x1 - 1 + x, y1 - 1 + y)] = direction;
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

bool Map::addPlayer(Player& player, uint32_t slot) {
	//Todo: Slot verwalten und prüfen ob schon belegt


	HumanPlayer* hPlayer = dynamic_cast<HumanPlayer*>(&player);

	if (hPlayer != 0)
		viewerManager.addHumanPlayer(hPlayer);

	return true;
}

void Map::removePlayer(Player& player, bool removeEntitys) {
	//Todo: Entitys entfernen

	HumanPlayer* hPlayer = dynamic_cast<HumanPlayer*>(&player);

	if (hPlayer != 0)
		viewerManager.removeHumanPlayer(hPlayer);

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
	uint32_t currentEndTime;

	while (movingUnits.size() > 0) {		MovingUnit* m = movingUnits.top();
		currentEndTime = m->getFinishTime();
		// Wenn Bewegung noch nicht fertig -> return		if (currentEndTime > currentTime)			return;
		// Bewegung ist abgeschlossen		m->finishMove(*this);		movingUnits.pop();

		// Prüfen ob wir eine weitere Bewegung machen sollen		if (m->getUnit().countWaypoints() > 0) {			std::cout << "Starte weitere bewegung\n";			Unit& unit = m->getUnit();
			viewerManager.debugPaintField(position(unit.getX(), unit.getY()), 0xFF50FF00);

			uint32_t pos = unit.getNextWaypoint();			uint8_t direction = Utils::getAngle(*this, position(unit.getX(), unit.getY()), pos);			m->startMove(direction, currentEndTime, *this);
			movingUnits.push(m);		}		else {
			viewerManager.debugPaintField(position(m->getUnit().getX(), m->getUnit().getY()), 0xFF289900);			delete m;		}	}
}