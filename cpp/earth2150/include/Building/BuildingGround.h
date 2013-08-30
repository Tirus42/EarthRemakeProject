#ifndef BUILDINGGROUND_H_INCLUDED
#define BUILDINGGROUND_H_INCLUDED

#include <stdint.h>
#include <memory.h>

class BuildingGround {
	private:
		/// Gebäude Breite (X Achse)
		uint8_t width;

		/// Gebäude Höhe (Y Achse)
		uint8_t height;

		/// Array der Felder (siehe GroundType)
		uint8_t* field;

		/// Berechnet den Index in dem Feld-Array
		size_t getIndex(uint8_t x, uint8_t y) const;

		BuildingGround(const BuildingGround& cc);
		BuildingGround& operator=(const BuildingGround& cc);

	public:
		/// Definition der Bodenflächenarten
		enum GroundType {
			FIELD_FREE,	// Gebäude blockiert/nutzt dieses Feld nicht
			FIELD_SOLID,	// Gebäude steht auf diesem Feld
			FIELD_ENTRY	// Von/auf dieses Feld fahren Einheiten, welche mit dem Gebäude interagieren
		};

		/// Konstruktor
		/// Erstellt die Untergrundfläche für Gebäude.
		/// Die Fläche wird mit dem Wert startType initiiert.
		BuildingGround(uint8_t width, uint8_t height, GroundType startType = FIELD_SOLID);

		/// Destruktor
		~BuildingGround();

		/// Gibt die Breite der Bodenfläche zurück
		uint8_t getWidth() const {
			return width;
		}

		/// Gibt die Höhe der Bodenfläche zurück
		uint8_t getHeight() const {
			return height;
		}

		/// Git den Typ der Bodenfläche zurück
		GroundType getField(uint8_t x, uint8_t y) const;

		/// Setzt den Typ eines Feldes
		void setField(uint8_t x, uint8_t y, GroundType type);

};

#endif // BUILDINGGROUND_H_INCLUDED
