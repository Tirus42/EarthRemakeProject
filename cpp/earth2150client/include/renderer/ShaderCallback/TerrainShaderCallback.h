#ifndef TERRAINSHADERCALLBACK_H_INCLUDED
#define TERRAINSHADERCALLBACK_H_INCLUDED

using namespace irr;
using namespace irr::video;

class TerrainShaderCallback : public video::IShaderConstantSetCallBack
{
public:

	virtual void OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
	{
		video::IVideoDriver* driver = services->getVideoDriver();

		core::matrix4 invWorld = driver->getTransform(video::ETS_WORLD);
		invWorld.makeInverse();

		services->setVertexShaderConstant("mInvWorld", invWorld.pointer(), 16);

		// Hole Koordinantes des Objekts Zentrums und übergebe diese an den Shader
		core::vector3df origin = driver->getTransform(video::ETS_WORLD).getTranslation();
		services->setVertexShaderConstant("mOrigin", &origin.X, 3);

		// Bestimme Model-View-Projection Matrix und übergebe an Shader
		core::matrix4 worldViewProj;
		worldViewProj = driver->getTransform(video::ETS_PROJECTION)
						* driver->getTransform(video::ETS_VIEW)
						* driver->getTransform(video::ETS_WORLD);

		services->setVertexShaderConstant("mMVP", worldViewProj.pointer(), 16);

		// set texture, for textures you can use both an int and a float setPixelShaderConstant interfaces (You need it only for an OpenGL driver).
		s32 TextureLayerID = 0;

		services->setPixelShaderConstant("myTexture", &TextureLayerID, 1);

	}
};

#endif // TERRAINSHADERCALLBACK_H_INCLUDED
