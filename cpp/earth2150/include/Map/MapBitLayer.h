#ifndef MAPBITLAYER_H
#define MAPBITLAYER_H

#include <stdint.h>

class MapBitLayer{
	private:
		uint16_t minX, minY;
		uint16_t width, height;
		uint32_t *bits;

		MapBitLayer(const MapBitLayer& cc);
		MapBitLayer& operator=(MapBitLayer& cc);
	public:
		MapBitLayer(uint16_t minX, uint16_t minY, uint16_t width, uint16_t height);
		~MapBitLayer();

		bool isSet(uint16_t x, uint16_t y) const{
			uint32_t bitNumber=(y-minY)*width+x-minX;
			return bits[bitNumber >> 5] & 1 << (bitNumber & 0x1F);
		}

		void set(uint16_t x, uint16_t y){
			uint32_t bitNumber=(y-minY)*width+(x-minX);
			bits[bitNumber >> 5] |= 1 << (bitNumber & 0x1F);
		}

		void unset(uint16_t x, uint16_t y){
			uint32_t bitNumber=(y-minY)*width+(x-minX);
			bits[bitNumber >> 5] &= ~(1 << (bitNumber & 0x1F));
		}

		uint32_t *getBits() const{return bits;}
};

#endif // MAPBITLAYER_H
