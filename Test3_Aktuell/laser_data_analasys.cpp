#include <iostream>
#include <string>
#include <regex>
#include <vector>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Fehler: Bitte den Datenstring als Argument übergeben!" << std::endl;
        return 1;
    }

    // Datenstring aus den Argumenten zusammenfügen
    std::string input;
    for (int i = 1; i < argc; ++i) {
        input += argv[i];
        if (i < argc - 1) input += " ";
    }
   
    // Marker definitions
    std::string startMarker = "---START---";
    std::string endMarker = "___END___";

    // Check if both markers exist
    size_t startPos = input.find(startMarker);
    size_t endPos = input.find(endMarker);

    if (startPos != std::string::npos && endPos != std::string::npos) {
        // Extract the data between markers
        startPos += startMarker.length();
        std::string jsonData = input.substr(startPos, endPos - startPos);

        // Find the ranges array in the JSON data
        std::regex rangesRegex("\"ranges\":\\s*\\[(.*?)\\]");
        std::smatch matches;
        if (std::regex_search(jsonData, matches, rangesRegex)) {
            std::string rangesString = matches[1].str();

            // Parse the ranges into a vector
            std::vector<float> ranges;
            std::regex numberRegex("-?\\d+(\\.\\d+)?");
            auto numbersBegin = std::sregex_iterator(rangesString.begin(), rangesString.end(), numberRegex);
            auto numbersEnd = std::sregex_iterator();

            for (std::sregex_iterator i = numbersBegin; i != numbersEnd; ++i) {
                ranges.push_back(std::stof((*i).str()));
            }

            // Check if the ranges array has exactly 360 values
            if (ranges.size() == 360) {
                std::cout << "Extracted ranges:" << std::endl;
                for (float range : ranges) {
                    std::cout << range << " ";
                }
                std::cout << std::endl;
            } else {
                std::cout << "Error: Ranges array does not contain exactly 360 values. Found: " << ranges.size() << std::endl;
            }
        } else {
            std::cout << "Ranges array not found in the JSON data." << std::endl;
        }
    } else {
        std::cout << "Markers not found in the input string." << std::endl;
    }

    return 0;
}
