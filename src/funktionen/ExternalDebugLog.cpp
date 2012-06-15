#include "ExternalDebugLog.h"
#include "network.h"

bool InitExternalDebugLog(const std::string server, unsigned short port=1000) {
    Socket stream = OpenTCPStream(server, port);

    if (stream != 0){
        debugConnection = stream;

        return true;
    }

    return false;
}


void WriteString(Socket socket, const std::string text) {

}
