#include "renderer/DeferredShadingScreenRenderer.h"

#include "renderer/ShaderCallback/GlobalLightCallback.h"
#include "renderer/ShaderCallback/PointLightCallback.h"
#include "renderer/ShaderCallback/TerrainShaderCallback.h"

#include "client/VisualMap.h"

#include "tf/time.h"

#include <cassert>

using namespace irr;

DeferredShadingScreenRenderer::DeferredShadingScreenRenderer(IrrlichtDevice* device, video::SColor backgroundColor) :
	IScreenRenderer(device, backgroundColor),
	helper(device->getVideoDriver()->getCurrentRenderTargetSize()),
	screenSize(device->getVideoDriver()->getCurrentRenderTargetSize()),
	renderTargets(),
	postProcessRenderTarget(0),
	usePostProcessRenderTarget(true),
	shaderMaterial(SHADER_COUNT),
	globalLightShaderCallback(new GlobalLightCallback()),
	pointLightShaderCallback(new PointLightCallback()) {

}

DeferredShadingScreenRenderer::~DeferredShadingScreenRenderer() {
	freeRenderTargets();

	globalLightShaderCallback->drop();
	pointLightShaderCallback->drop();
}

bool DeferredShadingScreenRenderer::init() {
	// Setzte Material Attribute


	for (u32 i = 0; i < SHADER_COUNT; ++i)
		shaderMaterial.push_back(video::SMaterial());

	// Deaktiviere Fixed Function Pipeline Lightning
	shaderMaterial[SHADER_GLOBALLIGHT].Lighting = false;
	shaderMaterial[SHADER_POINTLIGHT].Lighting = false;

	// Setzte passenden ZBuffer Operator und deakiviere schreiben für Lichtquellen
	shaderMaterial[SHADER_POINTLIGHT].ZBuffer = video::ECFN_GREATER;
	shaderMaterial[SHADER_POINTLIGHT].ZWriteEnable = false;

	resize(screenSize);

	return loadShaders();
}

void DeferredShadingScreenRenderer::resize(const irr::core::dimension2du& newSize) {
	// Gebe alte Render Targets wieder frei
	freeRenderTargets();

	this->screenSize = newSize;

	// Erstelle neue Render Targets
	createAndSetRenderTargets(newSize);
}

bool DeferredShadingScreenRenderer::createAndSetRenderTargets(const irr::core::dimension2du& size) {
	video::IVideoDriver* driver = device->getVideoDriver();

	// Erstelle Render Targets
	video::ITexture* DiffuseTex = driver->addRenderTargetTexture(size, "Diffuse", video::ECF_R8G8B8);
	video::ITexture* NormTex = driver->addRenderTargetTexture(size, "Normal", video::ECF_R8G8B8);
	video::ITexture* PosTex = driver->addRenderTargetTexture(size, "Position", video::ECF_A16B16G16R16F);

	if (usePostProcessRenderTarget)
		postProcessRenderTarget.RenderTexture = driver->addRenderTargetTexture(size, "PostProcess", video::ECF_A16B16G16R16F);

	if (DiffuseTex == 0 || NormTex == 0 || PosTex == 0) {
		device->getLogger()->log("Failed to create Render Targets!", ELL_ERROR);
		return false;
	}

	assert(renderTargets.size() == 0);

	// Füge diese der Liste an
	renderTargets.push_back(DiffuseTex);
	renderTargets.push_back(NormTex);
	renderTargets.push_back(PosTex);

	// Setze Texturen als eingabe für die Materials (Shader)
	shaderMaterial[SHADER_GLOBALLIGHT].setTexture(0, DiffuseTex);
	shaderMaterial[SHADER_GLOBALLIGHT].setTexture(1, NormTex);
	shaderMaterial[SHADER_GLOBALLIGHT].setTexture(2, PosTex);

	shaderMaterial[SHADER_POINTLIGHT].setTexture(0, DiffuseTex);
	shaderMaterial[SHADER_POINTLIGHT].setTexture(1, NormTex);
	shaderMaterial[SHADER_POINTLIGHT].setTexture(2, PosTex);

	return true;
}

void DeferredShadingScreenRenderer::freeRenderTargets() {
	video::IVideoDriver* driver = device->getVideoDriver();

	// Texturzuweisung entfernen
	for (u32 i = 0; i < SHADER_COUNT; ++i) {
		for (u32 j = 0; j < video::MATERIAL_MAX_TEXTURES; ++j) {
			shaderMaterial[i].setTexture(j, 0);
		}
	}

	// Texturen freigeben
	for (u32 i = 0; i < renderTargets.size(); ++i) {
		driver->removeTexture(renderTargets[i].RenderTexture);
	}

	renderTargets.clear();

	if (postProcessRenderTarget.RenderTexture) {
		driver->removeTexture(postProcessRenderTarget.RenderTexture);
		postProcessRenderTarget.RenderTexture = 0;
	}
}

bool DeferredShadingScreenRenderer::loadShaders() {
	io::path path("shaders/deferredshading/");

	// PFard bestimmen für richtigen Shader (OpenGL <-> DX9)
	switch (device->getVideoDriver()->getDriverType()) {
		case video::EDT_OPENGL:
			path += "opengl/";
			break;
		case video::EDT_DIRECT3D9:
			path += "dx9/";
			break;
		default:
			device->getLogger()->log("This driver does not support shaders!", ELL_ERROR);
			return false;
	}

	video::IGPUProgrammingServices* gpu = device->getVideoDriver()->getGPUProgrammingServices();

	const io::path vsFileName = path + "terrain.vert";
	const io::path psFileName = path + "terrain.frag";

	const io::path vsLightName = path + "globallight.vert";
	const io::path psLightName = path + "globallight.frag";

	const io::path vssLightName = path + "pointlight.vert";
	const io::path pssLightName = path + "pointlight.frag";

	s32 MaterialType_Map = 0;
	s32 MaterialType_GlobalLight = 0;
	s32 MaterialType_PointLight = 0;

	TerrainShaderCallback* mc = new TerrainShaderCallback();

	printf("Kompiliere Shader...");

	MaterialType_Map = gpu->addHighLevelShaderMaterialFromFiles(
		vsFileName, "vertexMain", video::EVST_VS_3_0,
		psFileName, "pixelMain", video::EPST_PS_3_0,
		mc, video::EMT_SOLID, 0, video::EGSL_DEFAULT);

	MaterialType_GlobalLight = gpu->addHighLevelShaderMaterialFromFiles(
		vsLightName, "vertexMain", video::EVST_VS_3_0,
		psLightName, "pixelMain", video::EPST_PS_3_0,
		globalLightShaderCallback, video::EMT_SOLID, 0, video::EGSL_DEFAULT);

	MaterialType_PointLight = gpu->addHighLevelShaderMaterialFromFiles(
		vssLightName, "vertexMain", video::EVST_VS_3_0,
		pssLightName, "pixelMain", video::EPST_PS_3_0,
		pointLightShaderCallback, video::EMT_TRANSPARENT_ADD_COLOR, 0, video::EGSL_DEFAULT);

	// Pointer wird von Irrlicht übernommen (siehe Reference Counting)
	mc->drop();

	if (MaterialType_Map == -1)
		printf("Konnte Map Shader nicht kompilieren!\n");

	if (MaterialType_PointLight == -1)
		printf("Konnte PointLight Shader nicht kompilieren!\n");

	if (MaterialType_GlobalLight == -1)
		printf("Konnte GlobalLight Shader nicht kompilieren!\n");

	if (MaterialType_Map == -1 || MaterialType_GlobalLight == -1 || MaterialType_PointLight == -1)
		exit(1);

	video::SMaterial& ground = getMaterial(SHADER_MAP);
	video::SMaterial& gLight = getMaterial(SHADER_GLOBALLIGHT);
	video::SMaterial& sLight = getMaterial(SHADER_POINTLIGHT);

	ground.MaterialType = (video::E_MATERIAL_TYPE)MaterialType_Map;

	gLight.MaterialType = (video::E_MATERIAL_TYPE)MaterialType_GlobalLight;
	gLight.ZWriteEnable = true;
	gLight.ZBuffer = video::ECFN_ALWAYS;

	sLight.MaterialType = (video::E_MATERIAL_TYPE)MaterialType_PointLight;
	sLight.ZWriteEnable = false;
	sLight.ZBuffer = video::ECFN_GREATER;
	//sLight.BlendOperation = video::EBO_ADD;
	//sLight.MaterialTypeParam = video::pack_textureBlendFunc(video::EBF_SRC_ALPHA, video::EBF_ONE_MINUS_SRC_ALPHA);
	sLight.BlendOperation = video::EBO_ADD;
	sLight.MaterialTypeParam = video::pack_textureBlendFunc(video::EBF_ONE_MINUS_DST_ALPHA, video::EBF_DST_ALPHA, video::EMFN_MODULATE_4X);


	printf("Shader erfolgreich kompiliert\n");
	return true;
}

void DeferredShadingScreenRenderer::render(const VisualMap& map) {
	video::IVideoDriver* driver = device->getVideoDriver();
    scene::ISceneManager* smgr = device->getSceneManager();
    gui::IGUIEnvironment* guienv = device->getGUIEnvironment();

    {	// Übernehme beleuchtungs Informationen
		const MapGlobalLight& mLight = map.getGlobalLight();

		globalLightShaderCallback->ambientColor = mLight.ambientColor;
		globalLightShaderCallback->lightColor = mLight.lightColor;
		globalLightShaderCallback->direction = mLight.direction;
    }

    uint64_t startTime, endTime;

    HighResolutionTime(&startTime);

	driver->beginScene(true, true, backgroundColor);

	// Setze mehrere Rendertargets für Ausgabe
	driver->setRenderTarget(renderTargets);

	smgr->drawAll();

	map.drawTerrain(driver);

	// Zeichne Globale Beleuchtung
	if (usePostProcessRenderTarget) {
		driver->setRenderTarget(postProcessRenderTarget.RenderTexture, true, false, backgroundColor);
	} else {
		driver->setRenderTarget(video::ERT_FRAME_BUFFER, false, false, backgroundColor);
	}
	//driver->setRenderTarget(texture[0].RenderTexture, false, false, backgroundColor);

	driver->setMaterial(shaderMaterial[SHADER_GLOBALLIGHT]);

	{
		core::triangle3df t1;
		core::triangle3df t2;

		helper.buildFullScreenQuad(t1, t2);

		driver->draw3DTriangle(t1);
		driver->draw3DTriangle(t2);
	}


	const float myRadius = 50;

	const scene::IGeometryCreator* g = smgr->getGeometryCreator();
	scene::IMesh* mesh = g->createSphereMesh(myRadius);

	driver->getMeshManipulator()->flipSurfaces(mesh);

	video::SMaterial testMat;
	testMat.ZBuffer = irr::video::ECFN_GREATER;

	driver->setMaterial(shaderMaterial[SHADER_POINTLIGHT]);
	//driver->setMaterial(testMat);

	const core::array<SPointLightData>& pointLights = lightManager.getPointLights();

	core::matrix4 transformMatrix;

	for (u32 i = 0; i < pointLights.size(); ++i) {
		pointLightShaderCallback->currentPointLight = pointLights[i];

		transformMatrix.setTranslation(pointLights[i].position);

		driver->setTransform(video::ETS_WORLD, transformMatrix);

		driver->drawMeshBuffer(mesh->getMeshBuffer(0));
	}

	mesh->drop();

	if (usePostProcessRenderTarget) {
		driver->setRenderTarget(video::ERT_FRAME_BUFFER, false, false, video::SColor(255, 0, 0, 0));
		driver->draw2DImage(postProcessRenderTarget.RenderTexture, core::position2di(0, 0), core::recti(0, 0, screenSize.Width, screenSize.Height));
	}


	//driver->draw2DImage(tex, core::position2di(0, 0));


	guienv->drawAll();

	driver->endScene();

	HighResolutionTime(&endTime);

	this->lastRenderTime = HighResolutionDiffNanoSec(startTime, endTime);
}

