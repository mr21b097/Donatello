#include <iostream>
#include <string>
#include <regex>

struct Pose {
    struct Position {
        double x, y, z;
    } position;

    struct Orientation {
        double x, y, z, w;
    } orientation;
};

struct Twist {
    struct Linear {
        double x, y, z;
    } linear;

    struct Angular {
        double x, y, z;
    } angular;
};

struct RobotState {
    Pose pose;
    Twist twist;
};

RobotState parseRobotState(const std::string& data) {
    RobotState state = {};

    // Regex für Position, Orientierung, Linear und Angular
    std::regex positionRegex("\"position\":\\s*\\{\\s*\"x\":\\s*([-\\d\\.eE]+),\\s*\"y\":\\s*([-\\d\\.eE]+),\\s*\"z\":\\s*([-\\d\\.eE]+)\\s*\\}");
    std::regex orientationRegex("\"orientation\":\\s*\\{\\s*\"x\":\\s*([-\\d\\.eE]+),\\s*\"y\":\\s*([-\\d\\.eE]+),\\s*\"z\":\\s*([-\\d\\.eE]+),\\s*\"w\":\\s*([-\\d\\.eE]+)\\s*\\}");
    std::regex linearRegex("\"linear\":\\s*\\{\\s*\"x\":\\s*([-\\d\\.eE]+),\\s*\"y\":\\s*([-\\d\\.eE]+),\\s*\"z\":\\s*([-\\d\\.eE]+)\\s*\\}");
    std::regex angularRegex("\"angular\":\\s*\\{\\s*\"x\":\\s*([-\\d\\.eE]+),\\s*\"y\":\\s*([-\\d\\.eE]+),\\s*\"z\":\\s*([-\\d\\.eE]+)\\s*\\}");

    std::smatch match;

    // Position parsen
    if (std::regex_search(data, match, positionRegex)) {
        state.pose.position.x = std::stod(match[1]);
        state.pose.position.y = std::stod(match[2]);
        state.pose.position.z = std::stod(match[3]);
    } else {
        std::cerr << "Position nicht gefunden!" << std::endl;
    }

    // Orientierung parsen
    if (std::regex_search(data, match, orientationRegex)) {
        state.pose.orientation.x = std::stod(match[1]);
        state.pose.orientation.y = std::stod(match[2]);
        state.pose.orientation.z = std::stod(match[3]);
        state.pose.orientation.w = std::stod(match[4]);
    } else {
        std::cerr << "Orientierung nicht gefunden!" << std::endl;
    }

    // Lineare Geschwindigkeit parsen
    if (std::regex_search(data, match, linearRegex)) {
        state.twist.linear.x = std::stod(match[1]);
        state.twist.linear.y = std::stod(match[2]);
        state.twist.linear.z = std::stod(match[3]);
    } else {
        std::cerr << "Lineare Geschwindigkeit nicht gefunden!" << std::endl;
    }

    // Angulare Geschwindigkeit parsen
    if (std::regex_search(data, match, angularRegex)) {
        state.twist.angular.x = std::stod(match[1]);
        state.twist.angular.y = std::stod(match[2]);
        state.twist.angular.z = std::stod(match[3]);
    } else {
        std::cerr << "Angulare Geschwindigkeit nicht gefunden!" << std::endl;
    }

    return state;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Fehler: Bitte den Datenstring als Argument übergeben!" << std::endl;
        return 1;
    }

    // Datenstring aus den Argumenten zusammenfügen
    std::string data;
    for (int i = 1; i < argc; ++i) {
        data += argv[i];
        if (i < argc - 1) data += " ";
    }

    // Überprüfen, ob Start und End Marker vorhanden sind
    size_t startPos = data.find("---START---");
    size_t endPos = data.find("___END___");

    if (startPos != std::string::npos && endPos != std::string::npos) {
        // Extrahiere den relevanten JSON-Teil
        std::string jsonData = data.substr(startPos + 11, endPos - (startPos + 11));

        // Roboterstatus-Daten extrahieren
        RobotState state = parseRobotState(jsonData);

        // Ergebnisse ausgeben
        std::cout << "Position: (" 
                  << state.pose.position.x << ", " 
                  << state.pose.position.y << ", " 
                  << state.pose.position.z << ")" << std::endl;

        std::cout << "Orientation: (" 
                  << state.pose.orientation.x << ", " 
                  << state.pose.orientation.y << ", " 
                  << state.pose.orientation.z << ", " 
                  << state.pose.orientation.w << ")" << std::endl;

        std::cout << "Linear Velocity: (" 
                  << state.twist.linear.x << ", " 
                  << state.twist.linear.y << ", " 
                  << state.twist.linear.z << ")" << std::endl;

        std::cout << "Angular Velocity: (" 
                  << state.twist.angular.x << ", " 
                  << state.twist.angular.y << ", " 
                  << state.twist.angular.z << ")" << std::endl;
    } else {
        std::cerr << "Fehler: Start oder End Marker fehlt!" << std::endl;
    }

    return 0;
}
