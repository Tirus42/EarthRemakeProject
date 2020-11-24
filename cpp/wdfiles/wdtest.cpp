#include <stdio.h>

#include <zlib.h>

#include <fstream>
#include <stdexcept>
#include <string.h>	// for memcmp()
#include <vector>
#include <stdint.h>

static const uint8_t ZLIB_HEADER[2] = {0x78, 0x9C};
static const uint8_t MAGIC_BYTES[8] = {0xff, 0xa1, 0xd0, 0x31, 0x57, 0x44, 0x00, 0x02};

union FirstHeader {
	uint8_t bytes[24];

	struct {
		uint8_t magic0[8];		// always 0xff 0xa1 0xd0 0x31 0x57 0x44 0x00 0x02
		uint8_t guid[16];

	} part;
};

int main(int argc, char* argv[]) {
	if (argc != 2) {
		printf("Please specify a wd file as cmd parameter\n");
		return 0;
	}

	const char* filename = argv[1];

	std::ifstream f(filename, std::ios::binary | std::ios::ate);

	if (!f.is_open())
		throw std::runtime_error("Failed to open file!");

	std::streampos fileSize = f.tellg();

	f.seekg(0, std::ios::beg);

	std::vector<uint8_t> content(fileSize);

	f.read(reinterpret_cast<char*>(content.data()), fileSize);
	f.close();

	uint8_t temp[1024 * 1024];
	FirstHeader fHeader;

	uLongf outputSize = sizeof(fHeader);

	if (uncompress((Bytef*)(&fHeader), &outputSize, content.data(), fileSize) != 0)
		throw std::runtime_error("Failed to unzip first header");

	if (outputSize != sizeof(fHeader))
		throw std::runtime_error("First header size mismatch");

	for (size_t i = 0; i < sizeof(MAGIC_BYTES); ++i) {
		if (fHeader.part.magic0[i] != MAGIC_BYTES[i])
			throw std::runtime_error("Unexpected magic bytes");
	}

	printf("GUID:");

	for (size_t i = 0; i < 16; ++i)
		printf("0x%2.2x ", fHeader.part.guid[i]);

	printf("\n");

	return 0;
}
