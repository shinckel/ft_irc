#include "utils/Parser.hpp"
#include <sstream>

std::vector<std::string> Parser::split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;

    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}

bool Parser::validateCommand(const std::vector<std::string>& tokens, const std::string& commandName, int minArgs) {
    if (tokens.empty() || tokens[0] != commandName) {
        return false; // Command name does not match
    }
    if ((int)tokens.size() - 1 < minArgs) {
        return false; // Not enough arguments
    }
    return true;
}
