#include "client/MapGlobalLight.h"

#include <stdio.h>

using namespace irr;

MapGlobalLight::MapGlobalLight() :
	ambientColor(0.1f, 0.1f, 0.1f),
	lightColor(0.2f, 0.2f, 0.2f),
	direction(0, 1, 0) {}

void MapGlobalLight::setLightAngle(float yaw, float pitch) {
	core::vector3df angle0(-pitch, 0, 0);
	core::vector3df angle1(0, 0, -yaw);

	yaw = (yaw) * core::DEGTORAD64;
	pitch = pitch * core::DEGTORAD64;

	//direction = angle.rotationToDirection(core::vector3df(0, 1, 0));

	direction.X = sin(yaw);
	direction.Y = cos(yaw) * cos(pitch);
	direction.Z = -sin(pitch);

	if (cos(yaw) < 0 && cos(pitch) < 0)
		direction.Y *= -1.f;

	direction.normalize();

	printf("=> %f %f %f\n", direction.X, direction.Y, direction.Z);
}
