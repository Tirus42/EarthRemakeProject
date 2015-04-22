#pragma once

#include <stdint.h>

/**
* Speichert und Verwaltet die Höhenkarte der Spielumgebung.
* Speichert dafür (n+1)*(n+1) Höhenpunkte, welche die Ecken der Spielfelder definieren.
*/
class HeightMap {
	private:
		uint16_t* heightData;

		uint16_t mapWidth;
		uint16_t mapHeight;

		uint16_t* allocateMemory(uint16_t mapWidth, uint16_t mapHeight) const;

		void freeMemory(uint16_t* ptr) const;

		HeightMap(const HeightMap&);
		HeightMap operator=(const HeightMap&);

	public:
		HeightMap(uint16_t mapWidth, uint16_t mapHeight);
		~HeightMap();

		void resize(uint16_t newWidth, uint16_t newHeight);

		const uint16_t& operator[](uint32_t index) const;
		uint16_t& operator[](uint32_t index);

		const uint16_t& operator()(uint16_t x, uint16_t y) const;

		uint16_t& operator()(uint16_t x, uint16_t y);
};
