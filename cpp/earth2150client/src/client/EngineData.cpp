#include "client/EngineData.h"

#include <stdio.h>

EngineData::EngineData() :
	device(0),
	config() {

}

EngineData::~EngineData() {
	if (device)
		device->drop();
}

void EngineData::setDevice(irr::IrrlichtDevice* device) {
	if (this->device)
		this->device->drop();

	this->device = device;

	if (this->device)
		this->device->grab();
}
