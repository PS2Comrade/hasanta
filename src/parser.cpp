#include "parser.h"


HasantaParser::HasantaParser() {
    initializeMaps();
}

void HasantaParser::initializeMaps() {
    // Exact Hasanta Phonetic Consonant Map
    consonants_ = {
        {"k", "ক"}, {"kh", "খ"}, {"g", "গ"}, {"gh", "ঘ"}, {"ng", "ঙ"},
        {"c", "চ"}, {"ch", "ছ"}, {"j", "জ"}, {"jh", "ঝ"}, {"NG", "ঞ"},
        {"T", "ট"}, {"Th", "ঠ"}, {"D", "ড"}, {"Dh", "ঢ"}, {"N", "ণ"},
        {"t", "ত"}, {"th", "থ"}, {"d", "দ"}, {"dh", "ধ"}, {"n", "ন"},
        {"p", "প"}, {"ph", "ফ"}, {"f", "ফ"}, {"b", "ব"}, {"bh", "ভ"}, {"v", "ভ"},
        {"m", "ম"}, {"z", "য"}, {"r", "র"}, {"l", "ল"},
        {"Sh", "ষ"}, {"sh", "শ"}, {"s", "স"}, {"h", "হ"},
        {"R", "ড়"}, {"Rh", "ঢ়"}, {"y", "য়"}, {"Y", "য়"}, {"q", "ক"},
        // Jofola triggers (rendered via auto-hasanta + jha)
        {"Z", "য"}
    };

    // Vowel Signs (Kar) - Attached when following a consonant
    vowelSigns_ = {
        {"a", "া"}, {"i", "ি"}, {"I", "ী"}, {"u", "ু"}, {"U", "ূ"},
        {"rR", "ৃ"}, {"e", "ে"}, {"OI", "ৈ"}, {"O", "ো"}, {"o", "ো"}, {"OU", "ৌ"}
    };

    // Independent Vowels - Used at word start or after another vowel
    independentVowels_ = {
        {"o", "অ"}, {"a", "আ"}, {"i", "ই"}, {"I", "ঈ"}, {"u", "উ"}, {"U", "ঊ"},
        {"rR", "ঋ"}, {"e", "এ"}, {"OI", "ঐ"}, {"O", "ও"}, {"OU", "ঔ"}
    };

    // Modifiers & Anusvara / Visarga rules
    specialCases_ = {
        {"ng", "ং"}, {"Ng", "ং"},
        {"th", "ঃ"},
        {"rri", "ৃ"},
        {"x", "ক্স"}
    };
}

bool HasantaParser::isConsonant(const std::string& token) const {
    return consonants_.find(token) != consonants_.end();
}

bool HasantaParser::isVowel(const std::string& token) const {
    return vowelSigns_.find(token) != vowelSigns_.end() || independentVowels_.find(token) != independentVowels_.end();
}

std::string HasantaParser::translate(const std::string& input) {
    std::string result = "";
    size_t i = 0;
    bool lastWasConsonant = false;
    std::string lastConsonantToken = "";

    while (i < input.length()) {
        bool matched = false;

        // Greedy Lookahead: Try matching 3 chars down to 1 char
        for (size_t len = 3; len >= 1; --len) {
            if (i + len > input.length()) continue;

            std::string token = input.substr(i, len);

            // Handle Special Cases / Modifiers first
            if (specialCases_.count(token)) {
                result += specialCases_[token];
                lastWasConsonant = false;
                i += len;
                matched = true;
                break;
            }

            // Handle Consonants
            if (isConsonant(token)) {
                // RULE: If last was a consonant and this is a consonant -> Inject Hasanta (্)
                // Exception: Hasanta doesn't link if explicitly broken or certain modifiers match
                if (lastWasConsonant) {
                    result += "্"; // U+09CD
                }
                result += consonants_[token];
                lastWasConsonant = true;
                lastConsonantToken = token;
                i += len;
                matched = true;
                break;
            }

            // Handle Vowels
            if (isVowel(token)) {
                if (lastWasConsonant) {
                    // Context check for "a" option: Hasanta drops "a" diacritic if it means inherent sound
                    if (token == "a") {
                        // In Hasanta, typing 'ka' gives 'কা', but some contexts omit it.
                        // For pure Hasanta compatibility, 'a' after consonant is 'া' (U+09BE)
                        result += vowelSigns_[token];
                    } else if (vowelSigns_.count(token)) {
                        result += vowelSigns_[token];
                    }
                } else {
                    // Standalone vowel
                    if (independentVowels_.count(token)) {
                        result += independentVowels_[token];
                    }
                }
                lastWasConsonant = false;
                i += len;
                matched = true;
                break;
            }
        }

        // If nothing matched, process it as punctuation/escape
                if (!matched) {
                    if (input[i] == '`') {
                        // The Hasanta escape hatch: Break the consonant link but don't print the tick
                        lastWasConsonant = false;
                    } else {
                        result += input[i];
                        lastWasConsonant = false;
                    }
                    i++;
                }
    }

    return result;
}
