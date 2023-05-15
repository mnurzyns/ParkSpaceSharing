#include <botan/argon2.h>
#include <botan/system_rng.h>

#include "PasswordUtils.hh"

namespace server {

std::string
PasswordUtils::hashPassword(std::string const& password)
{
    return Botan::argon2_generate_pwhash(
      password.c_str(),
      password.size(),
      Botan::system_rng(),
      1,
      65536, // TODO(papaj-na-wrotkach): move to config
      1);
}

bool
PasswordUtils::checkHash(std::string const& hash, std::string const& password)
{
    return Botan::argon2_check_pwhash(password.c_str(), password.size(), hash);
}

} // namespace server