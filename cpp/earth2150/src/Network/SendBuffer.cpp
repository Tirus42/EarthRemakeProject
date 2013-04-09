#include "Network/SendBuffer.h"

#include <cstring>

SendBuffer::SendBuffer(char* pointer, int32_t length) :
	buffer(0),
	length(length),
	offset(0) {

	buffer = new char[length];

	memcpy(buffer, pointer, length);
}

bool SendBuffer::sendPacket(SOCKET socket) {
	int32_t sendSize = socketSend(socket, buffer + offset, length - offset);

	if (sendSize == length - offset) {
		return true;
	}

	offset += sendSize;

	return false;
}

SendBuffer::~SendBuffer(){
	delete buffer;
}
