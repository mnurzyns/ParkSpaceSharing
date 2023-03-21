#pragma once

#include <oatpp/core/base/Environment.hpp>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/Types.hpp>

#include "dto/offer_dto.hh"
#include "dto/user_dto.hh"

namespace server::dto
{

#include OATPP_CODEGEN_BEGIN(DTO)

template<class T>
class page_dto :
    public ::oatpp::DTO
{
    DTO_INIT(page_dto, DTO)

    DTO_FIELD(Vector<T>, items);
};

class user_page_dto
    : public ::server::dto::page_dto<::oatpp::Object<::server::dto::user_dto>>
{
    DTO_INIT(user_page_dto, ::server::dto::page_dto<::oatpp::Object<::server::dto::user_dto>>)
};

class offer_page_dto
    : public ::server::dto::page_dto<::oatpp::Object<::server::dto::offer_dto>>
{
    DTO_INIT(offer_dto, ::server::dto::page_dto<::oatpp::Object<::server::dto::offer_dto>>)
};

#include OATPP_CODEGEN_END(DTO)

} // namespace server::dto