#ifndef UNITCHASSIS_H_INCLUDED
#define UNITCHASSIS_H_INCLUDED

#include <string>

/**
* Speichert die unterschliedlichen Unit-Chassis (z.B. Moon1, Moon2, USC Jaguar ect...)
*/
class UnitChassis {
	private:
		std::string id;    //"LCULU2"
		std::string name;  //"Lunar II"
		uint32_t turnRate; //Mikrosekunden um 1° zu drehen
		uint32_t moveRate; //Mikrosekunden um 1 Feld (1 Meter) zu fahren

		void setTurnRate(uint32_t timePerDegree) {turnRate = timePerDegree;}
		void setMoveRate(uint32_t timePerMeter) {moveRate = timePerMeter;}
	public:
		UnitChassis(const std::string& id, const std::string& name, uint32_t turnRate, uint32_t moveRate);

		uint32_t getTurnRate() const {return turnRate;}
		uint32_t getMoveRate() const {return moveRate;}
};

#endif
