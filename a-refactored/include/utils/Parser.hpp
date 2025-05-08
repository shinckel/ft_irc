#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include "core/Client.hpp"

class Parser {
public:
    static void processClientMessage(Client &client, const std::string &message);
};

#endif