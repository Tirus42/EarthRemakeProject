#ifndef SPOINTLIGHTDATA_H_INCLUDED
#define SPOINTLIGHTDATA_H_INCLUDED

/**
* Beschreibt eine Punktlichtquelle.
*/
class SPointLightData {
	public:
		/// Position der Lichtquelle in Weltkoordinaten
		irr::core::vector3df position;

		/// Farbe der Lichtquelle
		irr::video::SColorf color;

		/// Maximalen Radius den die Lichtquelle beeinflusst
		irr::f32 radius;

		SPointLightData() :
			position(0),
			color(0),
			radius(0) {}

		SPointLightData(const irr::core::vector3df& position, const irr::video::SColorf& color, irr::f32 radius) :
			position(position),
			color(color),
			radius(radius) {}

};


#endif // SPOINTLIGHTDATA_H_INCLUDED
