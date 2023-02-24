#pragma once

#include <oatpp/core/base/Environment.hpp>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/Types.hpp>

#include "dto/parking_space.hh"
#include "dto/user.hh"

namespace server::dto
{

#include OATPP_CODEGEN_BEGIN(DTO)

template<class T>
class all :
    public ::oatpp::DTO
{
    DTO_INIT(all, DTO)

    DTO_FIELD(Vector<T>, items);
};

class user_all
    : public ::server::dto::all<::oatpp::Object<::server::dto::user>>
{
    DTO_INIT(user_all, ::server::dto::all<::oatpp::Object<::server::dto::user>>)
};

class parking_space_all
    : public ::server::dto::all<::oatpp::Object<::server::dto::parking_space>>
{
    DTO_INIT(parking_space_all, ::server::dto::all<::oatpp::Object<::server::dto::parking_space>>)
};

#include OATPP_CODEGEN_END(DTO)

} // namespace server::dto
