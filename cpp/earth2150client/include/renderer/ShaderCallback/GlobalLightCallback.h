#ifndef GLOBALLIGHTCALLBACK_H_INCLUDED
#define GLOBALLIGHTCALLBACK_H_INCLUDED

using namespace irr;
using namespace irr::video;

/**
* Callback Klasse für den Shader welcher die Globale Beleuchtung berechnet.
*/
class GlobalLightCallBack : public video::IShaderConstantSetCallBack
{
public:

	virtual void OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
	{
		core::vector3df pos(0.f, 1.f, 0.f);

		services->setVertexShaderConstant("LightDirection", reinterpret_cast<f32*>(&pos), 3);

		// Lichtfarben setzen
		video::SColorf col(0.1f, 0.1f, 0.1f);
		video::SColorf ambientColor(0.1f, 0.1f, 0.1f);

		services->setVertexShaderConstant("LightColor", reinterpret_cast<f32*>(&col), 3);
		services->setVertexShaderConstant("AmbientColor", reinterpret_cast<f32*>(&ambientColor), 3);


		// set texture, for textures you can use both an int and a float setPixelShaderConstant interfaces (You need it only for an OpenGL driver).
		s32 TextureLayerID1 = 0;
		s32 TextureLayerID2 = 1;
		s32 TextureLayerID3 = 2;

		services->setPixelShaderConstant("DiffuseTex", &TextureLayerID1, 1);
		services->setPixelShaderConstant("NormalTex", &TextureLayerID2, 1);
		services->setPixelShaderConstant("PositionTex", &TextureLayerID3, 1);
	}
};

#endif // GLOBALLIGHTCALLBACK_H_INCLUDED
