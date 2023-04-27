#pragma once

#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/Types.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

namespace server::dto {

    class AuthDto : public oatpp::DTO {
        DTO_INIT(AuthDto, DTO)

        DTO_FIELD(oatpp::String, token, "token");

        DTO_FIELD(oatpp::String, tokenType, "tokenType");
    };

#include OATPP_CODEGEN_END(DTO)

} // namespace server::dto