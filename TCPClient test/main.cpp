#include "TCPClinet.h"
#include <iostream>

int main() {
    std::string ip = "192.168.100.54";

    TCPClient Test1(ip);

    
    Test1.receiveData();
    

    return 0;
}