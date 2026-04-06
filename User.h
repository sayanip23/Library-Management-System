#pragma once
#include <string>

class User
{
protected:
    std::string username;
    std::string password;

public:
    User();
    User(const std::string &uname, const std::string &pwd);
    std::string getUsername() const;
    std::string getPassword() const;
    bool authenticate(const std::string &enteredPassword) const;
};
