#ifndef LASER_SCAN_CLIENT_H
#define LASER_SCAN_CLIENT_H

#include <string>

class LaserScanClient {
public:
    LaserScanClient(const std::string& host, int port);
    void connectToServer();
    void readLaserData();
};

#endif // LASER_SCAN_CLIENT_H
