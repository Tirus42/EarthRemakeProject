#ifndef BUILDING_H_INCLUDED
#define BUILDING_H_INCLUDED

#include "e2150/Entity.h"

/**
* Diese Klasse stellt jedliches Spieler-Gebäude dar, welches in der Spielwelt gebaut wurde
*/
class Building : public Entity {
	private:
		static const uint8_t TYP_POWERSTATION;	//Kraftwerk
		static const uint8_t TYP_HOMEBASE;		//Hauptbasis / Hauptgebäude
		static const uint8_t TYP_FACTORY;		//Fabrik


		uint8_t typ;

	public:
		Building();
		virtual ~Building();

};

#endif // BUILDING_H_INCLUDED
