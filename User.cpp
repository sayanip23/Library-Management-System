#include "User.h"

User::User() : username(""), password("") {}

User::User(const std::string &uname, const std::string &pwd)
    : username(uname), password(pwd) {}

std::string User::getUsername() const
{
    return username;
}

std::string User::getPassword() const
{
    return password;
}

bool User::authenticate(const std::string &enteredPassword) const
{
    return password == enteredPassword;
}
