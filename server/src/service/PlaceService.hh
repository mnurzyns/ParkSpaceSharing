#pragma once

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/component.hpp>
#include <oatpp/web/protocol/http/Http.hpp>

#include "database/MainDatabase.hh"
#include "dto/PlaceDto.hh"
#include "dto/StatusDto.hh"

namespace server::service {

using namespace oatpp::data::mapping::type; // NOLINT
using namespace server::dto;                // NOLINT
using oatpp::Object, oatpp::web::protocol::http::HttpError,
  oatpp::web::protocol::http::Status;

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
    getPlacesByOwner(UInt64 const& id,   
           UInt64 const& limit = uint64_t{ 20 },
           UInt64 const& offset = uint64_t{ 0 });

    Object<PlacePageDto>
    search(String const& query,
           UInt64 const& limit = uint64_t{ 20 },
           UInt64 const& offset = uint64_t{ 0 });

    Object<PlaceDto>
    putOne(Object<PlaceDto> const& dto);

    Object<PlaceDto>
    patchOne(UInt64 const& id, Object<PlaceDto> const& dto);

    Object<StatusDto>
    deleteOne(UInt64 const& id);
};

} // namespace server::service
