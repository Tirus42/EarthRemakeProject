#pragma once

#include "UnitChassis.h"

#include <vector>
#include <stdint.h>

class UnitChassisList {
	private:
		std::vector<UnitChassis> chassisList;

		UnitChassisList(const UnitChassisList&);
		UnitChassisList operator=(const UnitChassisList&);

	public:
		UnitChassisList();

		uint32_t addChassis(const UnitChassis& chassis);

		const UnitChassis& getChassis(uint32_t index) const;

		UnitChassis& getChassis(uint32_t index);

		uint32_t getCount() const {
			return chassisList.size();
		}

};
