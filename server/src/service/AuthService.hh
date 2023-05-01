#pragma once

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/component.hpp>

#include "JWT.hh"
#include "database/MainDatabase.hh"
#include "dto/AuthDto.hh"
#include "dto/SignInDto.hh"
#include "dto/SignUpDto.hh"

namespace server::service {

using namespace oatpp::data::mapping::type;
using namespace server::dto;
using oatpp::Object, oatpp::web::protocol::http::Status;

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
