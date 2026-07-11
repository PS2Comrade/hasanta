#pragma once
#include <string>
#include <unordered_map>
#include <vector>

class HasantaParser {
public:
    HasantaParser();
    std::string translate(const std::string& input);

private:
    // Layout structures
    std::unordered_map<std::string, std::string> consonants_;
    std::unordered_map<std::string, std::string> vowelSigns_;
    std::unordered_map<std::string, std::string> independentVowels_;
    std::unordered_map<std::string, std::string> specialCases_;

    // Helper utilities
    bool isConsonant(const std::string& token) const;
    bool isVowel(const std::string& token) const;
    void initializeMaps();
};
