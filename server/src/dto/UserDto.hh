#pragma once

#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/Types.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

namespace server::dto {

    class UserDto : public oatpp::DTO {
        DTO_INIT(UserDto, DTO)

        DTO_FIELD(UInt64, id, "id");

        DTO_FIELD(String, username, "username");

        DTO_FIELD(String, email, "email");

        DTO_FIELD(String, password, "password");

        DTO_FIELD(Int32, role, "role"); // 0 - admin, 1 - user //TODO: enum
    };

} // namespace server::dto

#include OATPP_CODEGEN_END(DTO)
