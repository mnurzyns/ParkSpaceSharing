#pragma once

#include <oatpp/core/base/Environment.hpp>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/Types.hpp>

namespace server::dto
{

#include OATPP_CODEGEN_BEGIN(DTO)

class user_dto :
    public ::oatpp::DTO
{
    DTO_INIT(user_dto, DTO)

    DTO_FIELD(UInt32,  id);
    DTO_FIELD(String,  name,     "username");
    DTO_FIELD(String,  email,    "email");
    DTO_FIELD(String,  password, "password");
    DTO_FIELD(String,  token,    "token");
    DTO_FIELD(Boolean, admin,    "admin");
};

#include OATPP_CODEGEN_END(DTO)

} // namespace server::dto
