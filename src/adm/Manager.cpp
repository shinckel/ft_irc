#include "Manager.hpp"

std::vector<Client> Manager::_clients;
std::map<std::string, eventFunction> Manager::_actionMap;
std::map<std::string, Channel> Manager::_channels;
std::string Manager::_password;
std::string Manager::hostName = "localhost";
std::vector<std::string> Manager::_channelNames;

void Manager::addClient(int id){
    _clients.push_back(Client(id));
    std::cout << "Client " << id << " added to Clients" << std::endl;
}

void Manager::removeClient(int id){
    for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
        if (it->getId() == id) {
            _clients.erase(it);
            std::cout << "Client " << id << " removed from Clients" << std::endl;
            break;
        }
    }
}

// void Manager::removeClient(int id){
//     for(int i = 0; i < (int)_clients.size(); i++){
//         if(_clients[i].getId() == id){
//             _clients.erase(_clients.begin() + i);
//             std::cout << "Client " << id << " removed from Clients" << std::endl;
// 			break;
// 		}
//     }
// }

std::vector<Client> &Manager::getClient() {
	return (_clients);
}

std::stringstream &Manager::getClientBuffer(int i) {
	// DEBUGGING
	if (!isClient(i)) {
		std::cerr << "Error: Attempted to get buffer for non-existent client (fd: " << i << ")" << std::endl;
		throw std::runtime_error("Client does not exist");
	}
	// DEBUGGING

	std::vector<Client>::iterator it = _clients.begin();

	for ( ; it != _clients.end() ; it++) {
		if (it->getId() == i)
			break;
	}
	return (it->getBuffer());
}

std::vector<Client>::iterator Manager::getClientByID(int id) {
    for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
        if (it->getId() == id)
            return it;
    }
    return _clients.end(); // cliente não encontrado
}

// std::vector<Client>::iterator Manager::getClientByID(int i) {
// 	std::vector<Client>::iterator it = _clients.begin();

// 	for ( ; it != _clients.end() ; it++) {
// 		if (it->getId() == i)
// 			break;
// 	}
// 	return (it);
// }


std::map<std::string, eventFunction> &Manager::getActionMap(){
	return _actionMap;
}

std::map<std::string, Channel> &Manager::getChannels() {
	return _channels;
}

std::string Manager::getNickbyID(int id) {
	std::vector<Client>::iterator it = _clients.begin();
	for ( ; it != _clients.end() ; it++) {
		if (it->getId() == id)
			break;
	}
	return (it->getNickName());
}

int Manager::getIDbyNick(std::string nick) {
	std::vector<Client>::iterator it = _clients.begin();
	for ( ; it != _clients.end() ; it++) {
		if (it->getNickName() == nick)
			return (it->getId());
	}
	return (-1);
}

std::vector<int> Manager::getAllClientsIds() {
	std::vector<int> res;
	for(int i = 0; i < (int)_clients.size(); i++) {
		res.push_back(_clients[i].getId());
	}
	return (res);
}

std::string &Manager::getPassword() {
	return (_password);
}

std::vector<std::string> Manager::getChannelNames() {
	return (_channelNames);
}

void Manager::setPassword(std::string pass) {
	if (pass.length() > 0)
		_password = pass;
	else
		std::cout << "Password is empty" << std::endl;
}

//CHANNEL NAMES
void Manager::addChannelName(std::string name) {
	_channelNames.push_back(name);
	// std::cout << "LIST OF CHANNELS:" << std::endl;
	// for (int i = 0; i < (int) _channelNames.size(); i++) {
	// 	std::cout << "[" << i << "] = " <<_channelNames[i] << std::endl;
	// }
	// std::cout << "END OF LIST!" << std::endl;
}

int Manager::isClient(int id) {
	for (int i = 0; i < (int)_clients.size() ; i++) {
		if (_clients[i].getId() == id)
			return (1);
	}
	return (0);
}
