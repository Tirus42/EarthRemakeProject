#ifndef FRACTION_H
#define FRACTION_H

#include <stdint.h>
#include <string>

/**
* Jedes Objekt stellt eine Fraktion da (also Global wie USC, ED, LC ect)
*/
class Faction {
	private:
		static uint32_t lastUID;

		uint32_t uID;
		std::string name;
	public:
		Faction(const std::string& name);
		Faction(const std::string& name, uint32_t uID);
};

#endif
