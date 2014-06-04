#ifndef DEFERREDSHADINGSCREENRENDERER_H_INCLUDED
#define DEFERREDSHADINGSCREENRENDERER_H_INCLUDED

#include "renderer/IScreenRenderer.h"

#include "renderer/ScreenRendererHelper.h"

#include "renderer/LightManager.h"

class PointLightCallBack;

/**
* --- Groﬂe Baustelle ---
*/
class DeferredShadingScreenRenderer : public IScreenRenderer {
	private:
		ScreenRendererHelper helper;
		irr::core::dimension2du screenSize;

		irr::core::array<irr::video::IRenderTarget> renderTargets;

		LightManager lightManager;

	public:
		enum ShaderMaterial {
			SHADER_MAP = 0,
			SHADER_GLOBALLIGHT,
			SHADER_POINTLIGHT,
			SHADER_COUNT		// Anzahl der Shader
		};

		irr::core::array<irr::video::SMaterial> shaderMaterial;

		PointLightCallBack* pointLightShaderCallback;

	private:
		bool createAndSetRenderTargets(const irr::core::dimension2du& size);
		void freeRenderTargets();

		bool loadShaders();

	public:
		irr::video::SMaterial& getMaterial(ShaderMaterial mat) {
			return shaderMaterial[mat];
		}


	public:
		DeferredShadingScreenRenderer(irr::IrrlichtDevice* device, irr::video::SColor backgroundColor, const irr::core::dimension2du& screenSize);
		~DeferredShadingScreenRenderer();

		bool init();

		virtual void resize(const irr::core::dimension2du& newSize);

		/// Rendert die Szene auf den Bildschirm
		void render();

		LightManager& getLightManager() {
			return lightManager;
		}
};


#endif // DEFERREDSHADINGSCREENRENDERER_H_INCLUDED
