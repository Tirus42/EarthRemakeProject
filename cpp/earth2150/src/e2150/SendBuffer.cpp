#include "e2150/SendBuffer.h"

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
