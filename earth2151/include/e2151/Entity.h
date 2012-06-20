#ifndef ENTITY_H
#define ENTITY_H

#include "MapPosition.h"

class Entity {
    public:
        Entity();
        virtual ~Entity();
    private:
        int id;
        MapPosition mapPosition;
};

#endif
