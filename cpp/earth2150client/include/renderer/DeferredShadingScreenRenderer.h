#ifndef DEFERREDSHADINGSCREENRENDERER_H_INCLUDED
#define DEFERREDSHADINGSCREENRENDERER_H_INCLUDED

#include "renderer/IScreenRenderer.h"

#include "renderer/ScreenRendererHelper.h"

class GlobalLightCallback;
class PointLightCallback;

/**
* --- Groﬂe Baustelle ---
*/
class DeferredShadingScreenRenderer : public IScreenRenderer {
	private:
		ScreenRendererHelper helper;
		irr::core::dimension2du screenSize;

		irr::core::array<irr::video::IRenderTarget> renderTargets;

		irr::video::IRenderTarget postProcessRenderTarget;
		bool usePostProcessRenderTarget;

		DeferredShadingScreenRenderer(const DeferredShadingScreenRenderer&);
		DeferredShadingScreenRenderer& operator=(const DeferredShadingScreenRenderer&);
	public:
		enum ShaderMaterial {
			SHADER_MAP = 0,
			SHADER_GLOBALLIGHT,
			SHADER_POINTLIGHT,
			SHADER_COUNT		// Anzahl der Shader
		};

		irr::core::array<irr::video::SMaterial> shaderMaterial;

		GlobalLightCallback* globalLightShaderCallback;
		PointLightCallback* pointLightShaderCallback;

	private:
		bool createAndSetRenderTargets(const irr::core::dimension2du& size);
		void freeRenderTargets();

		bool loadShaders();

	public:
		irr::video::SMaterial& getMaterial(ShaderMaterial mat) {
			return shaderMaterial[mat];
		}


	public:
		DeferredShadingScreenRenderer(irr::IrrlichtDevice* device, irr::video::SColor backgroundColor);
		~DeferredShadingScreenRenderer();

		bool init();

		virtual void resize(const irr::core::dimension2du& newSize);

		/// Rendert die Szene auf den Bildschirm
		void render(const VisualMap& map);
};


#endif // DEFERREDSHADINGSCREENRENDERER_H_INCLUDED
