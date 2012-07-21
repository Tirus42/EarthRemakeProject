#ifndef SENDBUFFER_H_INCLUDED
#define SENDBUFFER_H_INCLUDED

#include <stdint.h>
#include <string.h>

#include <tf/network.h>

class SendBuffer{
    public:
        SendBuffer(char* pointer, int32_t length);
        ~SendBuffer();

        bool sendPacket(int32_t socket);

    private:
        SendBuffer(const SendBuffer &buffer);
        SendBuffer & operator=(const SendBuffer &buffer);

        char* buffer;
        int32_t length;
};

#endif // SENDBUFFER_H_INCLUDED
