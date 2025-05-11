#include "Client.hpp"

Client::Client(int id){
    _clientID = id;
    _nickName = "";
    _userName = "";
    _channel = "";
    _lastTriedNick = "";
    _authenticated = false;
    std::cout << "Client created with id: " << id << std::endl;
}

Client::~Client(){}

Client &Client::operator=(const Client &src) {
    _clientID = src._clientID;
    _nickName = src._nickName;
    _userName = src._userName;
    _channel = src._channel;
    _lastTriedNick = src._lastTriedNick;
    _cmd = src._cmd;
    _authenticated = src._authenticated;
    return *this;
}

Client::Client(const Client &src) {
    *this = src;
}

int Client::getId()  {
    return _clientID;
}

std::stringstream &Client::getBuffer()  {
	return (_buffer);
}

std::string &Client::getNickName()  {
    return (_nickName);
}

std::string &Client::getUserName()  {
    return (_userName);
}

std::vector<std::string> Client::getCommand(void)  {
   return (_cmd);
}

std::string &Client::getChannel()  {
    return _channel;
}

std::string &Client::getLastNick() {
    return _lastTriedNick;
}

std::string Client::getClientPrefix()  {
    std::string clientPrefix = getNickName() + "!" + \
        getUserName() + "@" + Manager::hostName;
    return clientPrefix;
}

void Client::setNickName(std::string nickName) {
    _nickName = nickName;
}


void Client::setUserName(std::string userName) {
    _userName = userName;
}


void Client::setHostName(std::string hostname) {
    _hostname = hostname;
}

void Client::setLastNick(std::string lastNick) {
    _lastTriedNick = lastNick;
}

void Client::setCommand(std::string cmd) {
    _cmd.clear();

    // Remove leading '/' if present
    if (!cmd.empty() && cmd[0] == '/')
        cmd.erase(0, 1);

    // Trim leading and trailing whitespace
    cmd.erase(0, cmd.find_first_not_of(" \t\r\n"));
    cmd.erase(cmd.find_last_not_of(" \t\r\n") + 1);

    // Split the command into parts
    size_t spacePos = cmd.find(" ");
    if (spacePos != std::string::npos) {
        std::string holder = Parser::toUpper(cmd.substr(0, spacePos)); // First word (uppercase)
        _cmd.push_back(holder);
        _cmd.push_back(cmd.substr(spacePos + 1)); // Rest of the command
    } else {
        std::string holder = Parser::toUpper(cmd); // Single-word command (uppercase)
        _cmd.push_back(holder);
    }

    // Debug output
    std::cout << "banana" << std::endl;
    std::cout << "Parsed command: ";
    for (unsigned long i = 0; i < _cmd.size(); i++) {
        std::cout << "[" << _cmd[i] << "] ";
    }
    std::cout << std::endl;
}

void Client::setregularCommand(std::string cmd) {
    _cmd.clear();
    if (cmd[0] == '/')
        cmd.erase(0, 1);

    _cmd.push_back(cmd);
}

void Client::setChannel(std::string channel) {
    _channel = channel;
}

// DEBUG PASSWORD
void Client::setAuthenticated(bool value) {
    _authenticated = value;  // Define o estado de autenticação
}

bool Client::isAuthenticated() const {
    return _authenticated;  // Retorna se o cliente está autenticado
}
