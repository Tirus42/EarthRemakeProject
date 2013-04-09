#ifndef SENDBUFFER_H_INCLUDED
#define SENDBUFFER_H_INCLUDED

#include <tf/network.h>
#include <stdint.h>

class SendBuffer {
	private:
		SendBuffer(const SendBuffer &buffer);
		SendBuffer& operator=(const SendBuffer &buffer);

		char* buffer;
		int32_t length;
		int32_t offset;

	public:
		SendBuffer(char* pointer, int32_t length);
		~SendBuffer();

		bool sendPacket(SOCKET socket);


};

#endif // SENDBUFFER_H_INCLUDED
