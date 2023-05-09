#pragma once

#include <oatpp/core/Types.hpp>

namespace server {

// RFC 5322 compliant email regex
static constexpr char const* EMAIL_REGEX =
  R"str(([!#-'*+/-9=?A-Z^-~-]+(\.[!#-'*+/-9=?A-Z^-~-]+)*|"([]!#-[^-~ \t]|(\\[\t -~]))+")@([!#-'*+/-9=?A-Z^-~-]+(\.[!#-'*+/-9=?A-Z^-~-]+)*|\[[\t -Z^-~]*]))str";

// RFC 3966 phone number regex
static constexpr char const* PHONE_REGEX =
  R"str(^\+?[1-9]\d{0,3}(?:[\s-]\d{2,}){1,4}$)str";

bool
validateEmail(char const* email);

bool
validatePhone(char const* phone);

} // namespace server
