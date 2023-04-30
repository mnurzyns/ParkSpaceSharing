#pragma once

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/component.hpp>
#include <oatpp/web/protocol/http/Http.hpp>

#include "database/MainDatabase.hh"
#include "dto/PlaceDto.hh"
#include "dto/StatusDto.hh"

namespace server::service {

using oatpp::Void, oatpp::Any, oatpp::String, oatpp::Int8, oatpp::UInt8,
  oatpp::Int16, oatpp::UInt16, oatpp::Int32, oatpp::UInt32, oatpp::Int64,
  oatpp::UInt64, oatpp::Float32, oatpp::Float64, oatpp::Boolean, oatpp::Object,
  oatpp::Enum, oatpp::Vector, oatpp::List, oatpp::UnorderedSet,
  oatpp::UnorderedMap, dto::PlaceDto, dto::PlacePageDto, dto::StatusDto;
using oatpp::web::protocol::http::HttpError;
using oatpp::web::protocol::http::Status;

class PlaceService
{

  private:
    OATPP_COMPONENT(std::shared_ptr<database::MainDatabase>,
                    database_); // NOLINT

  public:
    static std::shared_ptr<PlaceService>
    createShared();

    Object<PlaceDto>
    createOne(Object<PlaceDto> const& dto);

    Object<PlaceDto>
    getOne(UInt64 const& id);

    Object<PlacePageDto>
    search(String const& query,
           UInt64 const& limit = 20UL,
           UInt64 const& offset = 0UL);

    Object<PlaceDto>
    putOne(Object<PlaceDto> const& dto);

    Object<PlaceDto>
    patchOne(UInt64 const& id, Object<PlaceDto> const& dto);

    Object<dto::StatusDto>
    deleteOne(UInt64 const& id);
};

} // namespace server::service
