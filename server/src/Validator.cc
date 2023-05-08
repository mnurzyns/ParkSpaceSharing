#include "Validator.hh"

#include <oatpp/web/protocol/http/Http.hpp>

#include <regex>

namespace server {

void
ValidateEmailHTTP(oatpp::String const& email)
{
    std::regex email_rg{ EMAIL_REGEX };
    OATPP_ASSERT_HTTP(std::regex_search(*email, email_rg),
                      oatpp::web::protocol::http::Status::CODE_400,
                      "Invalid e-mail format")
}

void
ValidatePhoneHTTP(oatpp::String const& phone)
{
    std::regex phone_rg{ PHONE_REGEX };
    OATPP_ASSERT_HTTP(std::regex_search(*phone, phone_rg),
                      oatpp::web::protocol::http::Status::CODE_400,
                      "Invalid phone number format")
}

} // namespace server
