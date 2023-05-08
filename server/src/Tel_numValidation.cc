#include "Tel_numValidation.hh"

#include <oatpp/web/protocol/http/Http.hpp>

#include <regex>

namespace server {

void
validateTel_numHTTP(oatpp::String const& tel_num)
{
    std::regex telnum_rg{ TelNum_REGEX };
    OATPP_ASSERT_HTTP(std::regex_search(*tel_num, telnum_rg),
                      oatpp::web::protocol::http::Status::CODE_400,
                      "Invalid telephone number format")
}

} // namespace server