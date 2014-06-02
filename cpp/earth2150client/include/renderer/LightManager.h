#ifndef LIGHTMANAGER_H_INCLUDED
#define LIGHTMANAGER_H_INCLUDED

#include <irrlicht.h>

#include "renderer/LightData/SPointLightData.h"

/**
* Verwaltet Lichtquellen für das Rendering.
* Todo: Verwaltung der Indices verbessern
*/
class LightManager {
	private:
		irr::core::array<SPointLightData> pointLights;

		LightManager(const LightManager&);
		LightManager operator=(const LightManager&);

	public:
		LightManager();

		irr::u32 addPointLight(const irr::core::vector3df& position, const irr::video::SColorf& color, irr::f32 radius);

		// Todo...
		void removePointLight(irr::u32 index);

		SPointLightData& getPointLight(irr::u32 index) {
			return pointLights[index];
		}

		void clearPointLights();

		const irr::core::array<SPointLightData>& getPointLights() const {
			return pointLights;
		}

};


#endif // LIGHTMANAGER_H_INCLUDED
