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
};

#endif
