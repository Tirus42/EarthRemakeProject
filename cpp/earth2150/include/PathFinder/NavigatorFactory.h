#ifndef NAVIGATORFACTORY_H_INCLUDED
#define NAVIGATORFACTORY_H_INCLUDED

#include <stdint.h>

class INavigator;
class Map;

/**
* Factory Klasse um ein Wegfindungsalgorithmus-Objekt zu erstellen
*/
class NavigatorFactory {
	private:
		NavigatorFactory();

	public:
		static const uint8_t NAVIGATOR_ASTAR	= 0;
		static const uint8_t NAVIGATOR_JPS		= 1;
		static const uint8_t NAVIGATOR_BSEARCH	= 2;

		static const uint8_t NAVIGATOR_DEFAULT	= NAVIGATOR_JPS;

		static INavigator* getNavigator(const Map& map, uint8_t typ = NAVIGATOR_DEFAULT);
};

#endif // NAVIGATORFACTORY_H_INCLUDED
