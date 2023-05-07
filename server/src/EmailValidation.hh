#pragma once

#include <oatpp/core/Types.hpp>

namespace server {

// RFC 5322 compliant email regex
static constexpr char const* EMAIL_REGEX =
  R"str(([!#-'*+/-9=?A-Z^-~-]+(\.[!#-'*+/-9=?A-Z^-~-]+)*|"([]!#-[^-~ \t]|(\\[\t -~]))+")@([!#-'*+/-9=?A-Z^-~-]+(\.[!#-'*+/-9=?A-Z^-~-]+)*|\[[\t -Z^-~]*]))str";

void
validateEmailHTTP(oatpp::String const& email);

} // namespace server
