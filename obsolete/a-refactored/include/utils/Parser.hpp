#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <cstring>
#include <iostream>
#include <sstream>
#include <vector>

#include "core/Client.hpp"
#include "core/Manager.hpp"

class Parser
{
    public:
        static void processClientMessage(Client &client, const std::string &message);
        static std::vector<std::string> divideString(const std::string &str, char delim);
        static int isAction(const std::string &cmd, int i);
        static std::string toUpper(const std::string &str);
        static int nickParse(Client &client);
        static int inviteParse(Client &client);
        static int kickParse(Client &client);
        static int topicParse(Client &client);
        static int modeParse(Client &client);
        static int joinParse(Client &client);
        static int whoParse(Client &client);
        static int userParse(Client &client);
        static std::string &sanitizeBuffer(std::string &str);
};

#endif