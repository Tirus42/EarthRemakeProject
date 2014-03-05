#include "Player/PlayerRelationTable.h"

#include <memory.h>
#include <algorithm>
#include <cassert>

PlayerRelationTable::PlayerRelationTable(uint32_t countPlayer) :
	countPlayer(countPlayer),
	table(createTable(countPlayer)) {

}

PlayerRelationTable::~PlayerRelationTable() {
	removeTable(table);
}

void PlayerRelationTable::setRelation(uint32_t fromPlayer, uint32_t toPlayer, uint8_t relation) {
	assert(relation < RELATION_COUNT);

	uint32_t offset = getTableOffset(fromPlayer, toPlayer);

	table[offset] = relation;
}

void PlayerRelationTable::setRelationBidirectional(uint32_t player1, uint32_t player2, uint8_t relation) {
	setRelation(player1, player2, relation);
	setRelation(player2, player1, relation);
}

uint8_t PlayerRelationTable::getRelation(uint32_t fromPlayer, uint32_t toPlayer) const {
	uint32_t offset = getTableOffset(fromPlayer, toPlayer);

	return table[offset];
}

uint8_t* PlayerRelationTable::createTable(uint32_t countPlayer) const {
	uint8_t* newTable = new uint8_t[countPlayer * countPlayer];

	assert(newTable != 0);

	// Setzte Speicher zu 0 (Jeder ist Feind von jedem)
	memset(newTable, 0, countPlayer * countPlayer);

	return newTable;
}

void PlayerRelationTable::removeTable(uint8_t* table) const {
	delete[] table;
}

inline uint32_t getOffsetForSize(uint32_t fromPlayer, uint32_t toPlayer, uint32_t countPlayer) {
	assert(fromPlayer < countPlayer);
	assert(toPlayer < countPlayer);

	return (toPlayer * countPlayer + fromPlayer);
}

uint32_t PlayerRelationTable::getTableOffset(uint32_t fromPlayer, uint32_t toPlayer) const {
	return getOffsetForSize(fromPlayer, toPlayer, countPlayer);
}

void PlayerRelationTable::resize(uint32_t newCountPlayer) {
	// Erzeuge weitere Tabelle
	uint8_t* newTable = createTable(newCountPlayer);

	uint32_t countCopyablePlayer = std::min(countPlayer, newCountPlayer);

	// Kopiere Einträge
	for (uint32_t i = 0; i < countCopyablePlayer; ++i) {
		for (uint32_t j = 0; j < countCopyablePlayer; ++j) {
			uint32_t offsetOld = getOffsetForSize(i, j, countPlayer);
			uint32_t offsetNew = getOffsetForSize(i, j, newCountPlayer);

			newTable[offsetNew] = table[offsetOld];
		}
	}

	// Lösche alte Tabelle und weiße neue Daten zu
	removeTable(table);
	table = newTable;

	countPlayer = newCountPlayer;
}
