#ifndef MAPPOSITIONFLOAT_H_INCLUDED
#define MAPPOSITIONFLOAT_H_INCLUDED

#include "Map/MapPosition.h"

/**
* Stellt eine Position auf der Karte dar.
* Dabei wird die Verschiebung innerhalb eines Feldes zusätzlich gespeichert.
*/
class MapPositionFloat : public MapPosition {
	private:
		/// Beschreibt verschiebung innerhalb des Feldes [0, 1]
		float dx;
		float dy;

	public:
		/// Konstrutor, setzt verschiebung auf 0.5 (Mitte des Feldes)
		MapPositionFloat(uint16_t x, uint16_t y);
		MapPositionFloat(uint16_t x, uint16_t y, float dx, float dy);
		MapPositionFloat(float globalX, float globalY);

		/// Gibt die Verschiebung auf dem Feld in X-Richtung zurück
		float getDX() const {
			return dx;
		}

		/// Gibt die Verschiebung auf dem Feld in Y-Richtung zurück
		float getDY() const {
			return dy;
		}

		/// Setzt die X-Verschiebung
		void setDX(float dx) {
			this->dx = dx;
		}

		/// Setzt die Y-Verschiebung
		void setDY(float dy) {
			this->dy = dy;
		}

		/// Setzt die Verschiebung
		void setDXY(float dx, float dy) {
			this->dx = dx;
			this->dy = dy;
		}

};




#endif // MAPPOSITIONFLOAT_H_INCLUDED
