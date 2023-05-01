#pragma once

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>

#include "PageDto.hh"

#include OATPP_CODEGEN_BEGIN(DTO)

namespace server::dto {

using oatpp::Object;

class PlaceDto : public oatpp::DTO
{
    DTO_INIT(PlaceDto, DTO)

    DTO_FIELD(UInt64, id, "id");

    DTO_FIELD(UInt64, owner_id, "owner_id");

    DTO_FIELD(String, address, "address");

    DTO_FIELD(Float64, latitude, "latitude");

    DTO_FIELD(Float64, longitude, "longitude");
};

class PlacePageDto : public PageDto<Object<PlaceDto>>
{
    DTO_INIT(PlacePageDto, PageDto<Object<PlaceDto>>)
};

} // namespace server::dto

#include OATPP_CODEGEN_END(DTO)
