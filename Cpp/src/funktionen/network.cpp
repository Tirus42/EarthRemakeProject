#include "funktionen/network.h"

#ifdef WIN32
bool InitNetwork() {
    WORD wVersionRequested = MAKEWORD(2, 2);
    WSADATA wsaData;

    if(WSAStartup(wVersionRequested, &wsaData)){
        std::cout << "WSA Startup fehlgeschlagen!\n";
        return false;
    }
    return true;
}
#else
bool InitNetwork() {return true;};
#endif

int CreateTCPServer(unsigned short port, bool nonblock) {
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    sockaddr_in service;

    service.sin_family = AF_INET;
    service.sin_addr.s_addr = 0;    //inet_addr("0.0.0.0");
    service.sin_port = htons(port);

    int result = bind(sock, (sockaddr *) &service, sizeof(service));
    if(result == SOCKET_ERROR){
        return SOCKET_ERROR;
    }

    if(listen(sock, SOMAXCONN) == SOCKET_ERROR){
        std::cout << "Socket konnte nicht in listen mode gesetzt werden!";
    }

    if(nonblock){
        //Socket in non-Block mode setzen
        #ifdef WIN32
        u_long iMode = 1;
        result = ioctlsocket(sock, FIONBIO, &iMode);
        #else
        result = fcntl(sock, F_SETFL, fcntl(sock, F_GETFL, 0) | O_NONBLOCK);
        #endif
        if(result != NO_ERROR){
            std::cout << "Konnte Socket nicht in nonBlock mode setzen!\n";
        }
    }

    return sock;
}

int OpenTCPStream(const std::string& server, unsigned short port) {
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    sockaddr_in service;

    service.sin_family = AF_INET;
    service.sin_addr.s_addr = getHostIP(server);
    service.sin_port = htons(port);

    if(connect(sock, (sockaddr *) &service, sizeof(service)) != SOCKET_ERROR){
        return sock;
    }

    //cout << "Socket konnte nicht verbunden werden!\n";
    //cout << "Error: " << WSAGetLastError() << "\n";

    #ifdef WIN32
    closesocket(sock);
    #else
    close(sock);
    #endif
    return SOCKET_ERROR;
}

int CreateUDPStream(unsigned short port) {
    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    return sock;
}

void SendUDPMSG(const int socket, const char* data, int dataSize, uint32_t targetIP, unsigned short targetPort) {
    sockaddr_in service;

    service.sin_family = AF_INET;
    service.sin_addr.s_addr = targetIP;
    service.sin_port = htons(targetPort);

    sendto(socket, data, dataSize, 0, (sockaddr*)&service, sizeof(service));
}

void SendUDPMSG(const int socket, const char* data, int dataSize, const sockaddr_in target) {
    sendto(socket, data, dataSize, 0, (sockaddr*)&target, sizeof(target));
}

int getHostIP(const std::string& hostname) {
    #ifdef WIN32
    return inet_addr(hostname.c_str());
    #else
    //Linux macht das etwas komplizierter...
    struct hostent  *he;
    if ((he = gethostbyname(hostname.c_str())) == NULL) {
        return SOCKET_ERROR;
    }

    int addr;

    memcpy(&addr, he->h_addr_list[0], he->h_length);

    return addr;
    #endif
}