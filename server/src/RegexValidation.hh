#pragma once

#include <oatpp/core/Types.hpp>

namespace server {

// RFC 5322 compliant email regex
static constexpr char const* EMAIL_REGEX =
  R"str(([!#-'*+/-9=?A-Z^-~-]+(\.[!#-'*+/-9=?A-Z^-~-]+)*|"([]!#-[^-~ \t]|(\\[\t -~]))+")@([!#-'*+/-9=?A-Z^-~-]+(\.[!#-'*+/-9=?A-Z^-~-]+)*|\[[\t -Z^-~]*]))str";

void
validateEmailHTTP(oatpp::String const& email);

static constexpr char const* TelNum_REGEX =
  R"str(^[\+][0-9]{2,3}\s[0-9]{9}$)str";

void
validateTel_numHTTP(oatpp::String const& tel_num);

} // namespace server
