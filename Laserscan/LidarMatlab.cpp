#include "LidarProcessor.h"
#include "engine.h"  // MATLAB Engine API für C++
#include <iostream>
#include <cmath>
#include <vector>
#include <thread>  // Für die Verzögerung zwischen Aktualisierungen
#include <chrono>

//  "MATLAB Engine API" benötigt
//   MATLAB-Installationsverzeichnis unter:
//      MATLABroot/extern/include für die Header-Dateien.
//      MATLABroot/extern/lib für die Bibliotheken.



LidarProcessor::LidarProcessor(float minR, float maxR, float angleInc)
    : minRange(minR), maxRange(maxR), angleIncrement(angleInc) {}

std::vector<std::pair<float, float>> LidarProcessor::convertToCartesian(const std::vector<float>& ranges) {
    std::vector<std::pair<float, float>> cartesianPoints;
    for (size_t i = 0; i < ranges.size(); ++i) {
        float distance = ranges[i];
        if (distance < minRange || distance > maxRange || std::isnan(distance)) {
            continue;
        }

        float angle = i * angleIncrement;
        float x = distance * std::cos(angle);
        float y = distance * std::sin(angle);
        cartesianPoints.emplace_back(x, y);
    }
    return cartesianPoints;
}

void LidarProcessor::processLidarData(const std::vector<float>& ranges) {
    auto points = convertToCartesian(ranges);

    // Initialisiere die MATLAB-Engine
    Engine *ep = engOpen(NULL);
    if (ep == NULL) {
        std::cerr << "MATLAB Engine konnte nicht geöffnet werden!" << std::endl;
        return;
    }

    // Bereite die Daten für MATLAB vor
    std::vector<double> x_data, y_data;
    for (const auto& point : points) {
        x_data.push_back(point.first);
        y_data.push_back(point.second);
    }

    // Erstelle MATLAB-Arrays
    mxArray *x_mat = mxCreateDoubleMatrix(x_data.size(), 1, mxREAL);
    mxArray *y_mat = mxCreateDoubleMatrix(y_data.size(), 1, mxREAL);

    memcpy((void *)mxGetPr(x_mat), (void *)x_data.data(), sizeof(double) * x_data.size());
    memcpy((void *)mxGetPr(y_mat), (void *)y_data.data(), sizeof(double) * y_data.size());

    // Übergebe die Daten an MATLAB
    engPutVariable(ep, "x_data", x_mat);
    engPutVariable(ep, "y_data", y_mat);

    // Visualisiere die Daten in MATLAB
    engEvalString(ep, "scatter(x_data, y_data, 20, 'filled');");
    engEvalString(ep, "title('LIDAR Daten Visualisierung');");
    engEvalString(ep, "xlabel('X'); ylabel('Y');");

    // Halte den Prozess für eine kurze Zeit, bevor die nächsten Daten gesendet werden
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // MATLAB Engine schließen
    engClose(ep);
}
