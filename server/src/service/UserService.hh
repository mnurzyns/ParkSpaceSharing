#pragma once

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/component.hpp>
#include <oatpp/web/protocol/http/Http.hpp>

#include "database/MainDatabase.hh"
#include "dto/StatusDto.hh"
#include "dto/UserDto.hh"

namespace server::service {

using oatpp::Void, oatpp::Any, oatpp::String, oatpp::Int8, oatpp::UInt8,
  oatpp::Int16, oatpp::UInt16, oatpp::Int32, oatpp::UInt32, oatpp::Int64,
  oatpp::UInt64, oatpp::Float32, oatpp::Float64, oatpp::Boolean, oatpp::Object,
  oatpp::Enum, oatpp::Vector, oatpp::List, oatpp::UnorderedSet,
  oatpp::UnorderedMap, dto::UserDto, dto::UserPageDto, dto::StatusDto;
using oatpp::web::protocol::http::Status;

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
