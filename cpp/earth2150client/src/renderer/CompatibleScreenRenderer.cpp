#include "renderer/CompatibleScreenRenderer.h"

#include "client/VisualMap.h"

#include "tf/time.h"

using namespace irr;

CompatibleScreenRenderer::CompatibleScreenRenderer(irr::IrrlichtDevice* device, irr::video::SColor backgroundColor) :
	IScreenRenderer(device, backgroundColor) {


}

CompatibleScreenRenderer::~CompatibleScreenRenderer() {

}

bool CompatibleScreenRenderer::init() {
	// In diesem Renderer gibt es nichts zu initiieren
	return true;
}

void CompatibleScreenRenderer::resize(const irr::core::dimension2du& newSize) {
	// Nichts zu tun bei Auflösungsänderung
}

void CompatibleScreenRenderer::render(const VisualMap& map) {
	video::IVideoDriver* driver = device->getVideoDriver();
    scene::ISceneManager* smgr = device->getSceneManager();
    gui::IGUIEnvironment* guienv = device->getGUIEnvironment();

    uint64_t startTime, endTime;


    HighResolutionTime(&startTime);

	driver->beginScene(true, true, backgroundColor);

	map.drawTerrain(driver);

	smgr->drawAll();
	guienv->drawAll();

	driver->endScene();

	HighResolutionTime(&endTime);

	this->lastRenderTime = HighResolutionDiffNanoSec(startTime, endTime);
}
