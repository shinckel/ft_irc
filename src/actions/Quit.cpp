#include "Manager.hpp"

void Manager::removeClientFromChannels(Client &client) {
	for(int i = 0; i < (int)_channelNames.size(); i++){
		std::string channelName = _channelNames[i];
        if (_channels.find(channelName) != _channels.end() && _channels.find(channelName)->second.checkClient(client.getId())) {
			_channels.find(channelName)->second.removeClient(client.getId());
			_channels.find(channelName)->second.channelMessage(formatMessage(client) + " QUIT: has quit\r\n");
		}
    }
}

void Manager::quitAction(Client &client) {
	int id = client.getId();

	std::cout << "Sending QUIT message for client " << id << std::endl;
	sendIrcMessage(id, formatMessage(client) + " QUIT :Bye!");

	std::cout << "Removing client " << id << " from channels" << std::endl;
	removeClientFromChannels(client);

	std::cout << "Removing client " << id << " from Clients container" << std::endl;
	removeClient(id);
}

