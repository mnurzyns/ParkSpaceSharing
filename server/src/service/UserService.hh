#pragma once

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/component.hpp>
#include <oatpp/web/protocol/http/Http.hpp>

#include "database/MainDatabase.hh"
#include "dto/StatusDto.hh"
#include "dto/UserDto.hh"

namespace server::service {

using namespace oatpp::data::mapping::type; // NOLINT
using namespace server::dto;                // NOLINT
using oatpp::Object, oatpp::web::protocol::http::Status;

class UserService
{

  private:
    OATPP_COMPONENT(std::shared_ptr<database::MainDatabase>,
                    database_); // NOLINT

  public:
    static std::shared_ptr<UserService>
    createShared();

    Object<UserDto>
    createOne(Object<UserDto> const& dto);

    Object<UserDto>
    getOne(UInt64 const& id);

    Object<UserPageDto>
    search(String const& query,
           UInt64 const& limit = 20UL,
           UInt64 const& offset = 0UL);

    Object<UserDto>
    putOne(Object<UserDto> const& dto);

    Object<UserDto>
    patchOne(UInt64 const& id, Object<UserDto> const& dto);

    Object<StatusDto>
    deleteOne(UInt64 const& id);
};

} // namespace server::service
