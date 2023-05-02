#pragma once

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/component.hpp>

#include "TokenUtils.hh"
#include "database/MainDatabase.hh"
#include "dto/AuthDto.hh"
#include "dto/SignInDto.hh"
#include "dto/SignUpDto.hh"
#include "dto/StatusDto.hh"

namespace server::service {

using namespace oatpp::data::mapping::type; // NOLINT
using namespace server::dto;                // NOLINT
using oatpp::Object, oatpp::web::protocol::http::Status, dto::AuthDto,
  dto::SignInDto, dto::SignUpDto, dto::StatusDto;

class AuthService
{
  private:
    OATPP_COMPONENT(std::shared_ptr<database::MainDatabase>,
                    database_); // NOLINT
    OATPP_COMPONENT(std::shared_ptr<TokenUtils>,
                    token_utils_component); // NOLINT

  public:
    Object<AuthDto>
    signIn(Object<SignInDto> const& dto);

    Object<StatusDto>
    signUp(Object<SignUpDto> const& dto);
};

} // namespace server::service
