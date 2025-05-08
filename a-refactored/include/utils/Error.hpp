#pragma once
#include <string>
#include <map>

class Error {
public:
    static std::string getErrorMessage(int errorCode);

    // Error codes
    static const int ERR_NEEDMOREPARAMS;
    static const int ERR_NOSUCHNICK;
    static const int ERR_NOTONCHANNEL;
    static const int ERR_ALREADYREGISTRED;
    static const int ERR_PASSWDMISMATCH;

    // Reply codes
    static const int RPL_WELCOME;
    static const int RPL_WHOREPLY;
    static const int RPL_ENDOFWHO;

private:
    static std::map<int, std::string> initializeErrorMessages();
    static const std::map<int, std::string> errorMessages;
};
