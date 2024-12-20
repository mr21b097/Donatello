#ifndef VELOCITY_CLIENT_H
#define VELOCITY_CLIENT_H

#include <string>

class VelocityClient {
public:
    VelocityClient(const std::string& host, int port);
    void connectToServer();
    void sendVelocityCommand(float linear, float angular);
};

#endif // VELOCITY_CLIENT_H
