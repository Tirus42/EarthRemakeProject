#include "e2150/SendBuffer.h"

#ifdef WIN32
	#include <windows.h>
#else
	#include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>

    #include <fcntl.h>
    #include <netdb.h>
    #include <memory.h>

    #define NO_ERROR 0
    #define SOCKET_ERROR -1
#endif
#include <cstring>

SendBuffer::SendBuffer(char* pointer, int32_t length) :
    buffer(0),
    length(length){

    buffer = new char[length];

    memcpy(buffer, pointer, length);
}

bool SendBuffer::sendPacket(int32_t socket){
    if (send(socket, buffer, length, 0) == length){
        return true;
    }
    return false;
}

SendBuffer::~SendBuffer(){
    delete buffer;
}
