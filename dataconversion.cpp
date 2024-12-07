#include <iostream>
#include <regex>
#include <string>
#include <vector>

std::vector<int> extractIntegersRegex(const std::string &input) {
    std::vector<int> integers;
    std::regex numberPattern("\\d+"); // Muster: Eine oder mehrere Ziffern
    auto matchesBegin = std::sregex_iterator(input.begin(), input.end(), numberPattern);
    auto matchesEnd = std::sregex_iterator();

    for (auto it = matchesBegin; it != matchesEnd; ++it) {
        integers.push_back(std::stoi(it->str())); // Konvertiere Treffer zu Integer
    }

    return integers;
}

int main() {
    std::string input = "Das ist ein Teststring: 123352, 42524, 43534, 23455653, Test Test Test";
    std::vector<int> numbers = extractIntegersRegex(input);

    std::cout << "Gefundene Integer: ";
    for (int num : numbers) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    return 0;
}
