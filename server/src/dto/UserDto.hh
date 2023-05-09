#pragma once

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>

#include "PageDto.hh"

#include OATPP_CODEGEN_BEGIN(DTO)

namespace server::dto {

using oatpp::Object;

ENUM(Role, int8_t, VALUE(Admin, 0, "admin"), VALUE(User, 1, "user"))

class UserDto : public oatpp::DTO
{
    DTO_INIT(UserDto, DTO)

    DTO_FIELD(UInt64, id, "id");

    DTO_FIELD(String, email, "email");

    DTO_FIELD(String, phone, "phone");

    DTO_FIELD(String, username, "username");

    DTO_FIELD(String, password, "password");

    DTO_FIELD(Enum<Role>::AsNumber, role, "role");
};

class UserPageDto : public PageDto<Object<UserDto>>
{
    DTO_INIT(UserPageDto, PageDto<Object<UserDto>>)
};

} // namespace server::dto

#include OATPP_CODEGEN_END(DTO)
