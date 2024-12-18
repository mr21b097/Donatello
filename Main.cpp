#include <iostream>
#include <thread>
#include "LaserScanClient.cpp"
#include "OdometryClient.cpp"
#include "VelocityClient.cpp"
#include "SharedMemory.cpp"

void readLaserScan(LaserScanClient& laserClient) {
    laserClient.readLaserData();
}

void readOdometry(OdometryClient& odometryClient) {
    odometryClient.readOdometryData();
}

void sendVelocity(VelocityClient& velocityClient) {
    while (true) {
        float linear = 0.5;
        float angular = 0.1;
        velocityClient.sendVelocityCommand(linear, angular);
    }
}

int main() {
    SharedMemory sharedMemory;
    LaserScanClient laserClient("192.168.100.54", 9997);
    OdometryClient odometryClient("192.168.100.54", 9998);
    VelocityClient velocityClient("192.168.100.54", 9999);

    laserClient.connectToServer();
    odometryClient.connectToServer();
    velocityClient.connectToServer();

    std::thread laserThread(readLaserScan, std::ref(laserClient));
    std::thread odometryThread(readOdometry, std::ref(odometryClient));
    std::thread velocityThread(sendVelocity, std::ref(velocityClient));

    laserThread.join();
    odometryThread.join();
    velocityThread.join();

    return 0;
}
