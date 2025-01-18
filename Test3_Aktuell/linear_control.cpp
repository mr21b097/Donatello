#include <iostream>
#include <cmath>
#include <algorithm>

// Konstanten für die Regelparameter
const double kp = 1.0; // Proportionalitätskonstante für rho
const double ka = 2.0; // Proportionalitätskonstante für alpha
const double kb = -1.0; // Proportionalitätskonstante für beta

// Funktion zur Normalisierung des Winkels auf den Bereich [-pi, pi]
double normalize(double angle) {
    while (angle > M_PI) angle -= 2 * M_PI;
    while (angle < -M_PI) angle += 2 * M_PI;
    return angle;
}

int main(int argc, char* argv[]) {
    if (argc != 7) {
        std::cerr << "Usage: " << argv[0] << " x_robot y_robot theta_robot x_goal y_goal theta_goal\n";
        return 1;
    }

    // Eingabeargumente einlesen
    double x_robot = std::stod(argv[1]);
    double y_robot = std::stod(argv[2]);
    double theta_robot = std::stod(argv[3]) * M_PI / 180.0; // Grad in Radiant umrechnen
    double x_goal = std::stod(argv[4]);
    double y_goal = std::stod(argv[5]);
    double theta_goal = std::stod(argv[6]) * M_PI / 180.0; // Grad in Radiant umrechnen

    // Ziel-Delta-Werte berechnen
    double delta_x = x_goal - x_robot;
    double delta_y = y_goal - y_robot;
    double delta_theta = theta_goal - theta_robot;

    // Entfernung und Winkel berechnen
    double rho = std::sqrt(std::pow(delta_x, 2) + std::pow(delta_y, 2));
    double alpha = std::atan2(delta_y, delta_x) - theta_robot;
    double beta = delta_theta - alpha;

    // Winkel normalisieren
    alpha = normalize(alpha);
    beta = normalize(beta);

    // Geschwindigkeiten berechnen
    double v = 0.0;
    double w = 0.0;

    if (std::abs(alpha) <= M_PI / 2) {
        v = kp * rho;
        w = ka * alpha + kb * beta;
    } else {
        v = 0.2; // Konstante Vorwärtsgeschwindigkeit
        w = ka * alpha + kb * beta;
    }

    // Geschwindigkeiten begrenzen
    v = std::min(v, 0.22);
    w = std::min(w, 2.84);

    if (v < 0.18) {
        v = 0.18;
    }

    // Ergebnisse ausgeben
    std::cout << "Linear velocity (v): " << v << " m/s\n";
    std::cout << "Angular velocity (w): " << w << " rad/s\n";

    return 0;
}
