#pragma once

namespace server {

class PasswordUtils
{
  public:
    static std::string
    hashPassword(std::string const& password);

    static bool
    checkHash(std::string const& hash, std::string const& password);
};

} // namespace server