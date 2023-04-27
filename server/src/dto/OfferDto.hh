#pragma once

#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/Types.hpp>

#include "PageDto.hh"

#include OATPP_CODEGEN_BEGIN(DTO)

namespace server::dto {

    class OfferDto : public oatpp::DTO {
        DTO_INIT(OfferDto, DTO)

        DTO_FIELD(UInt64, id, "id");

        DTO_FIELD(UInt64, placeId, "place_id");

        DTO_FIELD(Int64, dateFrom, "date_from");

        DTO_FIELD(Int64, dateTo, "date_to");

        DTO_FIELD(String, description, "description");

        DTO_FIELD(UInt64, price, "price");
    };

    class OfferPageDto : public PageDto<oatpp::Object<OfferDto>> {
        DTO_INIT(OfferPageDto, PageDto<oatpp::Object<OfferDto>>)
    };

} // namespace server::dto

#include OATPP_CODEGEN_END(DTO)