#ifndef GLOBALLIGHTCALLBACK_H_INCLUDED
#define GLOBALLIGHTCALLBACK_H_INCLUDED

/**
* Callback Klasse für den Shader welcher die Globale Beleuchtung berechnet.
*/
class GlobalLightCallback : public irr::video::IShaderConstantSetCallBack
{
public:
	irr::video::SColorf lightColor;
	irr::video::SColorf ambientColor;
	irr::core::vector3df direction;

	GlobalLightCallback() :
		lightColor(0.1f, 0.1f, 0.1f),
		ambientColor(0.1f, 0.1f, 0.1f),
		direction(0.f, 1.f, 0.f) {}

	virtual void OnSetConstants(irr::video::IMaterialRendererServices* services, irr::s32 userData)
	{
		services->setPixelShaderConstant("LightDirection", reinterpret_cast<irr::f32*>(&direction), 3);

		services->setPixelShaderConstant("LightColor", reinterpret_cast<irr::f32*>(&lightColor), 3);
		services->setPixelShaderConstant("AmbientColor", reinterpret_cast<irr::f32*>(&ambientColor), 3);


		// set texture, for textures you can use both an int and a float setPixelShaderConstant interfaces (You need it only for an OpenGL driver).
		irr::s32 TextureLayerID1 = 0;
		irr::s32 TextureLayerID2 = 1;
		irr::s32 TextureLayerID3 = 2;

		services->setPixelShaderConstant("DiffuseTex", &TextureLayerID1, 1);
		services->setPixelShaderConstant("NormalTex", &TextureLayerID2, 1);
		services->setPixelShaderConstant("PositionTex", &TextureLayerID3, 1);
	}
};

#endif // GLOBALLIGHTCALLBACK_H_INCLUDED
