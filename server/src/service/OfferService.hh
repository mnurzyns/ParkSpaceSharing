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

class OfferService
{

  private:
    OATPP_COMPONENT(std::shared_ptr<database::MainDatabase>,
                    database_); // NOLINT

  public:
    static std::shared_ptr<OfferService>
    createShared();

    Object<OfferDto>
    createOne(Object<OfferDto> const& dto);

    Object<OfferDto>
    getOne(UInt64 const& id);

    Object<OfferPageDto>
    search(String const& query,
           UInt64 const& limit = uint64_t{ 20 },
           UInt64 const& offset = uint64_t{ 0 });

    Object<OfferDto>
    putOne(Object<OfferDto> const& dto);

    Object<OfferDto>
    patchOne(UInt64 const& id, Object<OfferDto> const& dto);

    Object<StatusDto>
    deleteOne(UInt64 const& id);
};

} // namespace server::service
