#include "string.h"


int stringToInt(const std::string& text){
    int value = 0;
    std::istringstream(text) >> value;
    return value;
}
