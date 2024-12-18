#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

class OdometryClient {
public:
    OdometryClient(const std::string& ip, int port) {
        sock = socket(AF_INET, SOCK_STREAM, 0);
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(port);
        server_addr.sin_addr.s_addr = inet_addr(ip.c_str());
    }

    ~OdometryClient() {
        close(sock);
    }

    void connectToServer() {
        if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
            std::cerr << "Connection failed" << std::endl;
            return;
        }
    }

    void readOdometryData() {
        char buffer[1024];
        while (true) {
            int bytes_received = recv(sock, buffer, sizeof(buffer), 0);
            if (bytes_received > 0) {
                // Receive Data and write to shared memory
                std::cout << "Received odometry data: " << buffer << std::endl;
            }
        }
    }

private:
    int sock;
    struct sockaddr_in server_addr;
};
