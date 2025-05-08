#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <sstream>
#include <vector>

class Client {
private:
    int _clientID;
    std::string _nickName;
    std::string _userName;
    std::string _channel;
    std::string _lastTriedNick;
    std::string _hostname;
    std::stringstream _buffer;
    std::vector<std::string> _commands; // Stores the commands
    // std::vector<std::string> _cmd;

public:
    Client(int id);
    ~Client();
    Client(const Client &src);
    Client &operator=(const Client &src);

    // Getters
    int getId() const;
    std::string &getNickName();
    std::string &getLastNick();
    std::string &getUserName();
    std::string &getChannel();
    std::string getClientPrefix() const;
    std::stringstream &getBuffer();
    std::vector<std::string> &getCommand();

    // Setters
    void setNickName(const std::string &nickName);
    void setLastNick(const std::string &lastNick);
    void setUserName(const std::string &userName);
    void setChannel(const std::string &channel);
    void setHostName(const std::string &hostname);
};

#endif
