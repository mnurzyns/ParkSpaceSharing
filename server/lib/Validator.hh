#pragma once

#include <string_view>

namespace server {

// RFC 5322 compliant email regex
static constexpr std::string_view EMAIL_REGEX =
  R"str(([!#-'*+/-9=?A-Z^-~-]+(\.[!#-'*+/-9=?A-Z^-~-]+)*|"([]!#-[^-~ \t]|(\\[\t -~]))+")@([!#-'*+/-9=?A-Z^-~-]+(\.[!#-'*+/-9=?A-Z^-~-]+)*|\[[\t -Z^-~]*]))str";

// RFC 3966 phone number regex
static constexpr std::string_view PHONE_REGEX =
  R"str(^\+?[1-9]\d{0,3}(?:[\s-]\d{2,}){1,4}$)str";

bool
validateEmail(std::string_view email);

bool
validatePhone(std::string_view phone);

} // namespace server
