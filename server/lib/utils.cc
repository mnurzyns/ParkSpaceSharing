#include "utils.hh"

#include <botan/argon2.h>
#include <botan/system_rng.h>
#include <re2/re2.h>

namespace server::utils {

bool
validateEmail(std::string_view email)
{
    return RE2::FullMatch(email, RE2{ EMAIL_REGEX });
}

bool
validatePhone(std::string_view phone)
{
    return RE2::FullMatch(phone, RE2{ PHONE_REGEX });
}


std::string
hashPassword(std::string_view password)
{
    return Botan::argon2_generate_pwhash(
      password.data(),
      password.size(),
      Botan::system_rng(),
      1,
      65536, // TODO(papaj-na-wrotkach): move to config
      1);
}

bool
checkHash(std::string_view hash, std::string_view password)
{
    return Botan::argon2_check_pwhash(password.data(), password.size(), hash.data());
}

} // namespace server::utils
