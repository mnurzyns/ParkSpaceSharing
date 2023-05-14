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

    DTO_FIELD(UInt64, date_from, "date_from");

    DTO_FIELD(UInt64, date_to, "date_to");

    DTO_FIELD(String, description, "description");

    DTO_FIELD(UInt64, price, "price");
};

class OfferSearchDto : public oatpp::DTO
{
    DTO_INIT(OfferSearchDto, DTO)

    DTO_FIELD(String, query, "query");

    DTO_FIELD(UInt64, place_id, "place_id");

    DTO_FIELD(UInt64, date_from, "date_from");

    DTO_FIELD(UInt64, date_to, "date_to");

    DTO_FIELD(UInt64, price_min, "price_min");

    DTO_FIELD(UInt64, price_max, "price_max");

    DTO_FIELD(UInt64, owner_id, "owner_id");

    DTO_FIELD(Float64, latitude, "latitude");

    DTO_FIELD(Float64, longitude, "longitude");

    DTO_FIELD(Float64, distance, "distance");

    DTO_FIELD(UInt64, limit, "limit");

    DTO_FIELD(UInt64, offset, "offset");
};

class OfferPageDto : public PageDto<Object<OfferDto>>
{
    DTO_INIT(OfferPageDto, PageDto<Object<OfferDto>>)
};

} // namespace server::dto

#include OATPP_CODEGEN_END(DTO)
