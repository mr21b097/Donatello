#include <iostream>
#include <cmath>
#include <vector>
#include <chrono>
#include <thread>
//#include <opencv2/opencv.hpp>

// Struktur für die Pose des Roboters
struct Pose {
    double x, y, theta;
};

// Funktion zur Simulation von zufälligen Pose-Daten
Pose generate_random_pose_data() {
    Pose pose;
    pose.x = static_cast<double>(rand() % 100);  // Zufällige x-Position
    pose.y = static_cast<double>(rand() % 100);  // Zufällige y-Position
    pose.theta = static_cast<double>(rand() % 360);  // Zufällige Ausrichtung (Theta)
    return pose;
}

// Funktion, die das Mapping aktualisiert (simuliert)
void update_mapping(std::vector<Pose>& poses) {
    // Eine neue Pose generieren und zur Liste hinzufügen
    poses.push_back(generate_random_pose_data());
}

// Funktion, die das Mapping visualisiert
void display_mapping(const std::vector<Pose>& poses) {
    // Erstelle ein leeres Bild (schwarz)
    cv::Mat map_image = cv::Mat::zeros(400, 400, CV_8UC3);

    // Zeichne jede Pose auf das Bild (als roten Punkt)
    for (const auto& pose : poses) {
        int x = static_cast<int>(pose.x * 4);  // Skalierung für die Darstellung
        int y = static_cast<int>(pose.y * 4);  // Skalierung für die Darstellung
        cv::circle(map_image, cv::Point(x, y), 5, cv::Scalar(0, 0, 255), -1);  // Rot
    }

    // Zeige das Bild im Fenster an
    cv::imshow("Mapping", map_image);
    cv::waitKey(1);  // Warte auf Tastendruck, aber aktualisiere schnell
}

// Hauptfunktion
int main() {
    std::vector<Pose> poses;  // Vector, der alle Posen des Roboters speichert
    bool running = true;

    // Initiale Zeit setzen
    auto last_time = std::chrono::high_resolution_clock::now();

    // Fenster für OpenCV öffnen
    cv::namedWindow("Mapping", cv::WINDOW_NORMAL);

    while (running) {
        // Aktuelle Zeit holen
        auto current_time = std::chrono::high_resolution_clock::now();
        // Berechnen, wie viel Zeit seit dem letzten Update vergangen ist
        std::chrono::duration<double> elapsed = current_time - last_time;

        // Alle 1 Sekunde das Mapping aktualisieren
        if (elapsed.count() >= 1.0) {
            update_mapping(poses);  // Mapping aktualisieren
            last_time = current_time;  // Zeit des letzten Updates setzen

            // Das Mapping visualisieren
            display_mapping(poses);
        }

        // Warten für eine kurze Zeit (Verhindert eine zu hohe CPU-Auslastung)
        std::this_thread::sleep_for(std::chrono::milliseconds(100));  // Sleep für 100ms
    }

    cv::destroyAllWindows();  // Schließe alle OpenCV-Fenster
    return 0;
}
