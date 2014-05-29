#include "renderer/RenderViewport.h"

#include <irrlicht.h>

using namespace irr;

RenderViewport::RenderViewport(const scene::ICameraSceneNode* camera, const VisualMap& map, const core::recti& viewport) :
	camera(camera),
	map(map),
	viewport(viewport) {

	camera->grab();
}

RenderViewport::~RenderViewport() {
	camera->drop();
}
