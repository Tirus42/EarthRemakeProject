#ifndef UNITCHASSIS_H_INCLUDED
#define UNITCHASSIS_H_INCLUDED

#include <string>

/**
* Speichert die unterschliedlichen Unit-Chassis (z.B. Moon1, Moon2, USC Jaguar ect...)
*/
class UnitChassis {
    private:
        uint32_t uID;
        std::string name;
        uint32_t turnRate;  //Mikrosekunden um 1° zu drehen
        uint32_t moveRate;  //Mikrosekunden um 1 Feld (1 Meter) zu fahren
        uint32_t moveRateDiag;  //Mikrosekunden um 1 Feld Diagonal zu fahren (1.41m)

        void setTurnRate(uint32_t timePerDegree) {turnRate = timePerDegree;}
        void setMoveRate(uint32_t timePerMeter) {
            moveRate = timePerMeter;
            moveRateDiag = (uint32_t)(timePerMeter * 1.41);
        }

    public:
        uint32_t getTurnRate() const{return turnRate;}
        uint32_t getMoveRate() const{return moveRate;}
        uint32_t getMoveRateDiag() const{return moveRateDiag;}

};

#endif
