#pragma once

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

namespace server::dto {

class SignUpDto : public oatpp::DTO
{
    DTO_INIT(SignUpDto, DTO)

    DTO_FIELD(oatpp::String, username, "username");

    DTO_FIELD(oatpp::String, phone, "phone");

    DTO_FIELD(oatpp::String, email, "email");

    DTO_FIELD(oatpp::String, password, "password");
};

#include OATPP_CODEGEN_END(DTO)

} // namespace server::dto