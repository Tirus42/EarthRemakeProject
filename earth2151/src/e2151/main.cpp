#include "tf/network.h"
#include <iostream>

//#include "Map.h"

int main(int argc, char *argv[]) {
    std::cout << "Hello world!" << std::endl;

    //Map* m = new Map(8192, 8192);

    InitNetwork();

    /* SOCKET udp = */ CreateUDPStream(12356);

    //int test;
    //std::cin >> test;
    return EXIT_SUCCESS;
}
