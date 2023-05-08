#pragma once

#include <oatpp/core/Types.hpp>

namespace server {

// RFC 3966 telephone number regex
static constexpr char const* TelNum_REGEX =
  R"str(^\+?[1-9]\d{0,3}(?:[\s-]\d{2,}){1,4}$)str";

void
validateTel_numHTTP(oatpp::String const& tel_num);

} // namespace server
