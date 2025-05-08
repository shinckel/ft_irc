#include "utils/Error.hpp"

const int Error::ERR_NEEDMOREPARAMS = 461;
const int Error::ERR_NOSUCHNICK = 401;
const int Error::ERR_NOTONCHANNEL = 442;
const int Error::ERR_ALREADYREGISTRED = 462;
const int Error::ERR_PASSWDMISMATCH = 464;

const int Error::RPL_WELCOME = 001;
const int Error::RPL_WHOREPLY = 352;
const int Error::RPL_ENDOFWHO = 315;

const std::map<int, std::string> Error::errorMessages = Error::initializeErrorMessages();

std::map<int, std::string> Error::initializeErrorMessages() {
    std::map<int, std::string> messages;

    // Error messages
    messages[ERR_NEEDMOREPARAMS] = "Not enough parameters";
    messages[ERR_NOSUCHNICK] = "No such nick/channel";
    messages[ERR_NOTONCHANNEL] = "You're not on that channel";
    messages[ERR_ALREADYREGISTRED] = "You may not reregister";
    messages[ERR_PASSWDMISMATCH] = "Password incorrect";

    // Reply messages
    messages[RPL_WELCOME] = "Welcome to the Internet Relay Network";
    messages[RPL_WHOREPLY] = "WHO reply";
    messages[RPL_ENDOFWHO] = "End of WHO list";

    return messages;
}

std::string Error::getErrorMessage(int errorCode) {
    std::map<int, std::string>::const_iterator it = errorMessages.find(errorCode);
    if (it != errorMessages.end()) {
        return it->second;
    }
    return "Unknown error";
}
