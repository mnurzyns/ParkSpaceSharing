#pragma once

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

namespace server::dto {

class AuthDto : public oatpp::DTO
{
    DTO_INIT(AuthDto, DTO)

    DTO_FIELD(oatpp::String, token, "token");

    DTO_FIELD(oatpp::String, token_type, "tokenType");
};

#include OATPP_CODEGEN_END(DTO)

} // namespace server::dto