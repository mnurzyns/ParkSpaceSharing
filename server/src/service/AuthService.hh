#pragma once

#include <oatpp/core/macro/component.hpp>

#include "auth/JWT.hh"
#include "database/MainDatabase.hh"
#include "dto/AuthDto.hh"
#include "dto/SignInDto.hh"
#include "dto/SignUpDto.hh"

using Status = oatpp::web::protocol::http::Status;

namespace server::service
{

class AuthService
{
private:
    OATPP_COMPONENT(std::shared_ptr<database::MainDatabase>, database_);
    OATPP_COMPONENT(std::shared_ptr<auth::JWT>, jwt_);


    static oatpp::Object<database::model::UserModel> signUpDtoToUserModel(const oatpp::Object<dto::SignUpDto> &dto) {
      auto model = database::model::UserModel::createShared();

      model->admin = false;
      model->email = dto->email;
      model->id = nullptr;
      model->password = dto->password;
      model->username = dto->username;

      return model;
    }

public:
    oatpp::Object<dto::AuthDto>
    signIn(oatpp::Object<dto::SignInDto> const& dto);

    oatpp::Object<server::dto::AuthDto>
    signUp(oatpp::Object<dto::SignUpDto> const& dto);

};

} // namespace server::service

