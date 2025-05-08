#pragma once
#include "../core/Client.hpp"
#include "../core/Channel.hpp"
#include <vector>
#include <string>

class Command {
public:
    virtual ~Command() {}

    // Executes the command
    virtual void execute(Client* client, const std::vector<std::string>& args) = 0;
};
