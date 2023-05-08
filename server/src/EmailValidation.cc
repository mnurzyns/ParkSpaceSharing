#include "EmailValidation.hh"

#include <oatpp/web/protocol/http/Http.hpp>

#include <regex>

namespace server {

void
validateEmailHTTP(oatpp::String const& email)
{
    std::regex email_rg{ EMAIL_REGEX };
    OATPP_ASSERT_HTTP(std::regex_search(*email, email_rg),
                      oatpp::web::protocol::http::Status::CODE_400,
                      "Invalid e-mail format")
}

} // namespace server
