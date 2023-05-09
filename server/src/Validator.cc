#include "Validator.hh"

#include <regex>

namespace server {

bool
validateEmail(char const* email)
{
    return std::regex_search(email, std::regex{ EMAIL_REGEX });
}

bool
validatePhone(char const* phone)
{
    return std::regex_search(phone, std::regex{ PHONE_REGEX });
}

} // namespace server
