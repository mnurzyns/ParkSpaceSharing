#pragma once

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/component.hpp>

#include "JWT.hh"
#include "database/MainDatabase.hh"
#include "dto/AuthDto.hh"
#include "dto/SignInDto.hh"
#include "dto/SignUpDto.hh"

namespace server::service {

using oatpp::Void, oatpp::Any, oatpp::String, oatpp::Int8, oatpp::UInt8,
  oatpp::Int16, oatpp::UInt16, oatpp::Int32, oatpp::UInt32, oatpp::Int64,
  oatpp::UInt64, oatpp::Float32, oatpp::Float64, oatpp::Boolean, oatpp::Object,
  oatpp::Enum, oatpp::Vector, oatpp::List, oatpp::UnorderedSet,
  oatpp::UnorderedMap, dto::AuthDto, dto::SignInDto, dto::SignUpDto,
  dto::UserDto;
using oatpp::web::protocol::http::Status;

class AuthService
{
  private:
    OATPP_COMPONENT(std::shared_ptr<database::MainDatabase>,
                    database_);                                  // NOLINT
    OATPP_COMPONENT(std::shared_ptr<JWT>, jwt_object_component); // NOLINT

  public:
    Object<AuthDto>
    signIn(Object<SignInDto> const& dto);

    Object<AuthDto>
    signUp(Object<SignUpDto> const& dto);
};

} // namespace server::service
