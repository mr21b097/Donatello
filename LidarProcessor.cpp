#include "LidarProcessor.h"
#include <iostream>
#include <cmath>

LidarProcessor::LidarProcessor(float minR, float maxR, float angleInc)
    : minRange(minR), maxRange(maxR), angleIncrement(angleInc) {}

// LIDAR-Daten in kartesische Koordinaten umwandeln
std::vector<std::pair<float, float>> LidarProcessor::convertToCartesian(const std::vector<float>& ranges) {
    std::vector<std::pair<float, float>> cartesianPoints;

    for (size_t i = 0; i < ranges.size(); ++i) {
        float distance = ranges[i];

        // Filtere ungültige Werte
        if (distance < minRange || distance > maxRange || std::isnan(distance)) {
            continue;
        }

        // Winkel berechnen (in Radiant)
        float angle = i * angleIncrement;

        // Kartesische Koordinaten berechnen
        float x = distance * std::cos(angle);
        float y = distance * std::sin(angle);

        cartesianPoints.emplace_back(x, y);
    }

    return cartesianPoints;
}

// LIDAR-Daten verarbeiten
void LidarProcessor::processLidarData(const std::vector<float>& ranges) {
    auto points = convertToCartesian(ranges);

    std::cout << "Verarbeitete LIDAR-Daten (kartesische Koordinaten):\n";
    for (const auto& point : points) {
        std::cout << "x: " << point.first << ", y: " << point.second << "\n";
    }
}
