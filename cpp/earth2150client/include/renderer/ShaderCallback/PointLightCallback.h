#ifndef POINTLIGHTCALLBACK_H_INCLUDED
#define POINTLIGHTCALLBACK_H_INCLUDED

#include <IShaderConstantSetCallBack.h>

#include "renderer/LightData/SPointLightData.h"

using namespace irr;

class PointLightCallback : public irr::video::IShaderConstantSetCallBack
{
public:
	SPointLightData currentPointLight;

	PointLightCallback() :
		currentPointLight() {}

	virtual void OnSetConstants(irr::video::IMaterialRendererServices* services, irr::s32 userData)
	{
		video::IVideoDriver* driver = services->getVideoDriver();

		s32 TextureLayerID1 = 0;
		s32 TextureLayerID2 = 1;
		s32 TextureLayerID3 = 2;

		services->setPixelShaderConstant("DiffuseTex", &TextureLayerID1, 1);
		services->setPixelShaderConstant("NormalTex", &TextureLayerID2, 1);
		services->setPixelShaderConstant("PositionTex", &TextureLayerID3, 1);


		video::SColorf color(currentPointLight.color);

		services->setPixelShaderConstant("LightColor", reinterpret_cast<f32*>(&color), 3);
		services->setPixelShaderConstant("LightPosition", reinterpret_cast<f32*>(&(currentPointLight.position)), 3);

		services->setPixelShaderConstant("strength", &(currentPointLight.radius), 1);
		//services->setPixelShaderConstant("constantAttenuation", &(pointLight->constantAttenuation), 1);
		//services->setPixelShaderConstant("linearAttenuation", &(pointLight->linearAttenuation), 1);
		//services->setPixelShaderConstant("quadAttenuation", &(pointLight->quadAttenuation), 1);
	}
};

#endif // POINTLIGHTCALLBACK_H_INCLUDED
