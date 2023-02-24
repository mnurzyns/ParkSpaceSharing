#pragma once

#include <oatpp/core/base/Environment.hpp>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/Types.hpp>

namespace server::dto
{

#include OATPP_CODEGEN_BEGIN(DTO)

class user :
    public ::oatpp::DTO
{
    DTO_INIT(user, DTO)

    DTO_FIELD(UInt32, id);
    DTO_FIELD(String, name, "username");
    DTO_FIELD(String, password, "password");
};

#include OATPP_CODEGEN_END(DTO)

} // namespace server::dto
