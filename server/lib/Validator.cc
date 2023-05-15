#include "Validator.hh"

#include <re2/re2.h>

namespace server {

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

} // namespace server
