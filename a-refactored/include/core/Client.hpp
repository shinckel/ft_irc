#pragma once
#include <string>
#include <vector>

class Client {
public:
    explicit Client(int fd);
    ~Client();

    int getFd() const;
    std::string receiveMessage();
    void sendMessage(const std::string& message);

    // Methods for handling raw commands
    void setRawCommand(const std::string& rawCommand);
    const std::string& getRawCommand() const;
    const std::vector<std::string>& getCommandTokens() const;

private:
    int _fd;
    std::string _rawCommand; // Stores the raw command string
    std::vector<std::string> _commandTokens; // Stores the split tokens of the command
};
