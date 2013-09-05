#include "e2150/FactionManager.h"

#include <cassert>

FactionManager::FactionManager() :
	faction() {

}

FactionManager::~FactionManager() {

}

bool FactionManager::isValidIndex(uint32_t index) const {
	return (index < faction.size());
}

uint32_t FactionManager::getNextFreeID() const {
	return faction.size();
}

const Faction& FactionManager::getFaction(uint32_t index) const {
	assert(isValidIndex(index));

	return faction[index];
}

Faction& FactionManager::getFaction(uint32_t index) {
	assert(isValidIndex(index));

	return faction[index];
}

uint32_t FactionManager::addFaction(const std::wstring& name) {
    const uint32_t id = getNextFreeID();

	Faction fa(id, name);

	bool result = addFaction(fa);

	assert(result);
	(void)result;	// Verhindern das im Release eine unused variable Warnung auftritt

	return id;
}

bool FactionManager::addFaction(uint32_t id, const std::wstring& name) {
	Faction fa(id, name);

	return addFaction(fa);
}

bool FactionManager::addFaction(const Faction& newFaction) {
	if (newFaction.getID() < faction.size())
		return false;

	if (newFaction.getID() == faction.size()) {
		faction.push_back(newFaction);
		return true;
	}

    faction.reserve(newFaction.getID());
    faction[newFaction.getID()] = newFaction;
    return true;
}
