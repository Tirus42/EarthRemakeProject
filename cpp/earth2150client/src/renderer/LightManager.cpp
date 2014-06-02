#include "renderer/LightManager.h"

using namespace irr;

LightManager::LightManager() :
	pointLights() {

}


u32 LightManager::addPointLight(const irr::core::vector3df& position, const irr::video::SColorf& color, irr::f32 radius) {
	pointLights.push_back(SPointLightData(position, color, radius));

	return pointLights.size() - 1;
}


void LightManager::clearPointLights() {
	pointLights.clear();
}
