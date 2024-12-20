#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <windows.h>  // für Sleep() unter Windows
#include <cmath>      // für sqrt()
#include <sstream>
#include <iomanip>

std::string generate_random_pose_data() {
    // Zufällige Positionsdaten generieren
    double x = static_cast<double>(rand()) / RAND_MAX;
    double y = static_cast<double>(rand()) / RAND_MAX;
    double z = 0.0;

    // Zufällige Orientierungsdaten generieren
    double qx = 0.0;
    double qy = 0.0;
    double qz = (static_cast<double>(rand()) / RAND_MAX) * 2 - 1; // Zwischen -1 und 1
    double qw = sqrt(1.0 - qz * qz); // Wählen, dass Quaternion gültig bleibt

    std::ostringstream ss;
    ss << "---START---{"
       << "\"header\": {\"seq\": " << rand() % 100000 << ", \"stamp\": {\"secs\": " << time(NULL) << ", \"nsecs\": " << rand() % 1000000000 << "}, \"frame_id\": \"odom\"}, "
       << "\"child_frame_id\": \"base_footprint\", "
       << "\"pose\": {\"pose\": {\"position\": {\"x\": " << x << ", \"y\": " << y << ", \"z\": " << z << "}, \"orientation\": {\"x\": " << qx << ", \"y\": " << qy << ", \"z\": " << qz << ", \"w\": " << qw << "}}}, "
       << "\"twist\": {\"twist\": {\"linear\": {\"x\": 0.0, \"y\": 0.0, \"z\": 0.0}, \"angular\": {\"x\": 0.0, \"y\": 0.0, \"z\": " << (static_cast<double>(rand()) / RAND_MAX) * 0.1 << "}}}}"
       << "}END";
    return ss.str();
}

std::string generate_random_scan_data() {
    // Zufällige LIDAR-Daten simulieren
    std::ostringstream ss;
    ss << "---START---{"
       << "\"header\": {\"seq\": " << rand() % 100000 << ", \"stamp\": {\"secs\": " << time(NULL) << ", \"nsecs\": " << rand() % 1000000000 << "}, \"frame_id\": \"base_scan\"}, "
       << "\"angle_min\": 0.0, "
       << "\"angle_max\": 6.2657318115234375, "
       << "\"angle_increment\": 0.01745329238474369, "
       << "\"time_increment\": 0.0005592841189354658, "
       << "\"scan_time\": 0.20134228467941284, "
       << "\"range_min\": 0.12, "
       << "\"range_max\": 3.5, "
       << "\"ranges\": [";

    for (int i = 0; i < 360; ++i) {
        double range = static_cast<double>(rand()) / RAND_MAX * 3.5;
        ss << range;
        if (i < 359) ss << ", ";
    }
    
    ss << "]"
       << "}END";
    return ss.str();
}

int main() {
    // Initialisierung des Zufallszahlengenerators
    srand(time(0));

    // Endlosschleife zum Simulieren von Daten
    while (true) {
        // Nachrichten zufällig von Port 9997 oder 9998 generieren
        int port = (rand() % 2 == 0) ? 9997 : 9998;
        if (port == 9997) {
            std::cout << "Port 9997 Empfangen: " << generate_random_scan_data() << std::endl;
        } else {
            std::cout << "Port 9998 Empfangen: " << generate_random_pose_data() << std::endl;
        }

        // Eine Sekunde warten, bevor neue Daten generiert werden
        Sleep(1000); // Sleep in Millisekunden (1000 ms = 1 Sekunde)
    }

    return 0;
}
