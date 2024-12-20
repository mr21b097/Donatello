#ifndef ODOMETRY_CLIENT_H
#define ODOMETRY_CLIENT_H

#include <string>

class OdometryClient {
public:
    OdometryClient(const std::string& host, int port);
    void connectToServer();
    void readOdometryData();
};

#endif // ODOMETRY_CLIENT_H
