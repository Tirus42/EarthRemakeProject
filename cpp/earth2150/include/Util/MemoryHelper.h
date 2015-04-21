#pragma once

#include <stdint.h>

namespace memory {
	uint8_t peekUInt8LE(const void* ptr);

	uint16_t peekUInt16LE(const void* ptr);

	uint32_t peekUInt32LE(const void* ptr);

	uint64_t peekUInt64LE(const void* ptr);

	void pokeUInt8LE(void* ptr, uint8_t value);

	void pokeUInt16LE(void* ptr, uint16_t value);

	void pokeUInt32LE(void* ptr, uint32_t value);

	void pokeUInt64LE(void* ptr, uint64_t value);
}
