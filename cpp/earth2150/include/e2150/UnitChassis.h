#ifndef UNITCHASSIS_H_INCLUDED
#define UNITCHASSIS_H_INCLUDED

#include <stdint.h>
#include <string>

/**
* Speichert die unterschliedlichen Unit-Chassis (z.B. Moon1, Moon2, USC Jaguar ect...)
*/
class UnitChassis {
	private:
		uint32_t id;		//ID zur Laufzeit (nicht global eindeutig!)
		std::string model;	//Modellname der Einheit wie "LCULU2"
		std::string name;	//Name der einheit wie "Lunar II"

		uint32_t turnRate;	//Millisekunden um 1° zu drehen
		uint32_t moveRate;	//Millisekunden um 1 Feld (1 Meter) zu fahren

		uint32_t buildTime;	//Millisekunden um dieses Chassis zu fertigen (Bauzeit)

		int32_t hitPoints;	// Lebenspunkte einer Einheit (bewusst als signed-int gewählt)

		void setTurnRate(uint32_t timePerDegree) {turnRate = timePerDegree;}
		void setMoveRate(uint32_t timePerMeter) {moveRate = timePerMeter;}
		void setBuildTime(uint32_t buildTime) {this->buildTime = buildTime;}
	public:
		UnitChassis(uint32_t id, const std::string& model, const std::string& name,
					uint32_t turnRate, uint32_t moveRate, uint32_t buildTime,
					uint32_t hitPoints);

		uint32_t getID() const {return id;}
		const std::string& getModel() const {return model;}
		const std::string& getName() const {return name;}

		uint32_t getTurnRate() const {return turnRate;}
		uint32_t getMoveRate() const {return moveRate;}

		uint32_t getBuildTime() const {return buildTime;}

		uint32_t getHitPoints() const {return hitPoints;}
};

#endif
