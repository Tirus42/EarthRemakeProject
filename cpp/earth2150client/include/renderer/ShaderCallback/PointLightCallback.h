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

		// set clip matrix

		irr::core::matrix4 worldViewProj;
		worldViewProj = driver->getTransform(video::ETS_PROJECTION);
		worldViewProj *= driver->getTransform(video::ETS_VIEW);
		worldViewProj *= driver->getTransform(video::ETS_WORLD);


		//services->setVertexShaderConstant("mWorldViewProj", worldViewProj.pointer(), 16);


		/// World View
		core::matrix4 worldView = driver->getTransform(video::ETS_WORLD) * driver->getTransform(video::ETS_VIEW);

		services->setVertexShaderConstant("mWorldView", worldView.pointer(), 16);


		// set camera position

		// set transposed world matrix

		core::matrix4 world = driver->getTransform(video::ETS_WORLD);
		world = world.getTransposed();


		services->setVertexShaderConstant("mTransWorld", world.pointer(), 16);

		// set texture, for textures you can use both an int and a float setPixelShaderConstant interfaces (You need it only for an OpenGL driver).
		s32 TextureLayerID1 = 0;
		s32 TextureLayerID2 = 1;
		s32 TextureLayerID3 = 2;

		services->setPixelShaderConstant("DiffuseTex", &TextureLayerID1, 1);
		services->setPixelShaderConstant("NormalTex", &TextureLayerID2, 1);
		services->setPixelShaderConstant("PositionTex", &TextureLayerID3, 1);

		//services->setPixelShaderConstant("LightColor",
		//		reinterpret_cast<f32*>(&(pointLight->color)), 4);

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
