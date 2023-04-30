#pragma once

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>

#include "PageDto.hh"

#include OATPP_CODEGEN_BEGIN(DTO)

namespace server::dto {

using oatpp::Object;

class OfferDto : public oatpp::DTO
{
    DTO_INIT(OfferDto, DTO)

    DTO_FIELD(UInt64, id, "id");

    DTO_FIELD(UInt64, place_id, "place_id");

    DTO_FIELD(Int64, date_from, "date_from");

    DTO_FIELD(Int64, date_to, "date_to");

    DTO_FIELD(String, description, "description");

    DTO_FIELD(UInt64, price, "price");
};

class OfferPageDto : public PageDto<Object<OfferDto>>
{
    DTO_INIT(OfferPageDto, PageDto<Object<OfferDto>>)
};

} // namespace server::dto

#include OATPP_CODEGEN_END(DTO)