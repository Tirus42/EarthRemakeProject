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

		// set inverted world matrix
		// if we are using highlevel shaders (the user can select this when
		// starting the program), we must set the constants by name.

		core::matrix4 invWorld = driver->getTransform(video::ETS_WORLD);
		invWorld.makeInverse();


		services->setVertexShaderConstant("mInvWorld", invWorld.pointer(), 16);


		// set clip matrix

		core::matrix4 worldViewProj;
		worldViewProj = driver->getTransform(video::ETS_PROJECTION);
		worldViewProj *= driver->getTransform(video::ETS_VIEW);
		worldViewProj *= driver->getTransform(video::ETS_WORLD);


		services->setVertexShaderConstant("mWorldViewProj", worldViewProj.pointer(), 16);


		/* // set transposed world matrix

		core::matrix4 world = driver->getTransform(video::ETS_WORLD);
		world = world.getTransposed();


		services->setVertexShaderConstant("mTransWorld", world.pointer(), 16);

		/*/
		// set texture, for textures you can use both an int and a float setPixelShaderConstant interfaces (You need it only for an OpenGL driver).
		s32 TextureLayerID = 0;

		services->setPixelShaderConstant("myTexture", &TextureLayerID, 1);
		//*/

	}
};

#endif // TERRAINSHADERCALLBACK_H_INCLUDED
