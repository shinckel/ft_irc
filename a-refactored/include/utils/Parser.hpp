#pragma once
#include <string>
#include <vector>

class Parser {
public:
    // Splitting tokens
    static std::vector<std::string> split(const std::string& str, char delimiter);

    // Command validation
    static bool validateCommand(const std::vector<std::string>& tokens, const std::string& commandName, int minArgs);
};
