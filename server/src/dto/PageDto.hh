#pragma once

#include <oatpp/core/base/Environment.hpp>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/Types.hpp>

#include "UserDto.hh"

namespace server::dto
{

#include OATPP_CODEGEN_BEGIN(DTO)

template<class T>
class PageDto :
    public oatpp::DTO
{
    DTO_INIT(PageDto, DTO)

    DTO_FIELD(Vector<T>, content);
    DTO_FIELD(UInt64, total);
    DTO_FIELD(UInt64, offset);
    DTO_FIELD(UInt64, limit);

};

class UserSearchPageDto :
    public server::dto::PageDto<oatpp::Object<UserDto>>
{
    DTO_INIT(UserSearchPageDto, server::dto::PageDto<oatpp::Object<UserDto>>)
};

#include OATPP_CODEGEN_END(DTO)

} // namespace server::dto
