#include "e2151/Faction.h"

uint32_t Faction::lastUID = 0;

Faction::Faction(const std::string& name) :
		uID(lastUID),
		name(name) {
    lastUID++;
}

Faction::Faction(const std::string& name, uint32_t uID) :
		uID(uID),
		name(name) {
}
