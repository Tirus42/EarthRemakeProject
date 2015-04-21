#include "Util/MemoryHelper.h"

namespace memory {

// Todo: Prüfen ob Endianess auch auf BigEndian Rechnern so korrekt, ggf. anpassen

uint8_t peekUInt8LE(const void* ptr) {
	const uint8_t* p = (const uint8_t*)ptr;

	return p[0];
}

uint16_t peekUInt16LE(const void* ptr) {
	const uint8_t* p = (const uint8_t*)ptr;

	return p[1] << 8 | p[0];
}

uint32_t peekUInt32LE(const void* ptr) {
	const uint8_t* p = (const uint8_t*)ptr;

	return p[3] << 24 | p[2] << 16 | p[1] << 8 | p[0];
}

uint64_t peekUInt64LE(const void* ptr) {
	const uint8_t* p = (const uint8_t*)ptr;

	return (uint64_t)p[7] << 56 | (uint64_t)p[6] << 48 | (uint64_t)p[5] << 40 | (uint64_t)p[4] << 32
		   | p[3] << 24 | p[2] << 16 | p[1] << 8 | p[0];
}

// Todo: Behandlung bei BE Maschienen
// Todo: Beachten von Memory Alignment -> Kann zu Problemen führen

void pokeUInt8LE(void* ptr, uint8_t value) {
	*(uint8_t*)ptr = value;
}

void pokeUInt16LE(void* ptr, uint16_t value) {
	*(uint16_t*)ptr = value;
}

void pokeUInt32LE(void* ptr, uint32_t value) {
	*(uint32_t*)ptr = value;
}

void pokeUInt64LE(void* ptr, uint64_t value) {
	*(uint64_t*)ptr = value;
}

}
