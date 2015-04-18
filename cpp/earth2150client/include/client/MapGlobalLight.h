#pragma once

#include <SColor.h>
#include <vector3d.h>

/**
* Bestimmt die belichtung einer Spielkarte.
* Definiert ist das Umgebungslicht, sowie ein Richtungssicht (eine Sonne),
* sowie deren einstrahlrichtung.
*/
struct MapGlobalLight {
	/// Globale Beleuchtungsfarbe und Intensität
	irr::video::SColorf ambientColor;

	/// Farbe des (Sonnen-)Lichts und Intensität
	irr::video::SColorf lightColor;

	/// Richtung des (Sonnen-)Lichts
	irr::core::vector3df direction;

	/// Standardkonstruktur
	MapGlobalLight();

	/**
	* Setzt den Richtungsvektor abhängig von 2 Winkeln.
	* yaw = 0, pitch = 0 => Lichtquelle steht mittig oben
	* Winkelangaben in Grad (DEG)
	* \param yaw Lichtstand über X-Achse (-90 => links/West, 90 => rechts/Ost)
	* \param pitch Lichtstand über Y-Achse (-90 => Oben/Nord, 90 => unten/Süd)
	*/
	void setLightAngle(float yaw, float pitch);
};
