#include "renderer/NormalScreenRenderer.h"

#include "tf/time.h"

using namespace irr;

NormalScreenRenderer::NormalScreenRenderer(irr::IrrlichtDevice* device, irr::video::SColor backgroundColor) :
	IScreenRenderer(device, backgroundColor) {


}

NormalScreenRenderer::~NormalScreenRenderer() {

}

void NormalScreenRenderer::render() {
	video::IVideoDriver* driver = device->getVideoDriver();
    scene::ISceneManager* smgr = device->getSceneManager();
    gui::IGUIEnvironment* guienv = device->getGUIEnvironment();

    uint64_t startTime, endTime;


    HighResolutionTime(&startTime);

	driver->beginScene(true, true, backgroundColor);

	smgr->drawAll();
	guienv->drawAll();

	driver->endScene();

	HighResolutionTime(&endTime);

	this->lastRenderTime = HighResolutionDiffNanoSec(startTime, endTime);
}
