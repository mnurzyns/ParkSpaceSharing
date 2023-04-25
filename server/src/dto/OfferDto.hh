#pragma once

#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/Types.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

namespace server::dto {

    class OfferDto : public oatpp::DTO {
        DTO_INIT(OfferDto, DTO)

        DTO_FIELD(UInt64, id, "id");

        DTO_FIELD(UInt64, placeId, "placeId");

        DTO_FIELD(String, description, "description");

        DTO_FIELD(UInt64, price, "price");

        DTO_FIELD(UInt64, startDate, "startDate");

        DTO_FIELD(UInt64, endDate, "endDate");
    };

} // namespace server::dto

#include OATPP_CODEGEN_END(DTO)