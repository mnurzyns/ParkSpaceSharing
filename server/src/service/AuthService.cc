#include "AuthService.hh"

namespace server::service
{

oatpp::Object<server::dto::AuthDto> AuthService::signUp(oatpp::Object<server::dto::SignUpDto> const& dto) {

    {
        auto result = database_->getUserByEmail(dto->email);
        OATPP_ASSERT_HTTP(result->isSuccess(), Status::CODE_500, result->getErrorMessage());
        OATPP_ASSERT_HTTP(!result->hasMoreToFetch(), Status::CODE_409, "EMAIL_ALREADY_EXISTS");
    }
    {
        auto result = database_->getUserByUsername(dto->username);
        OATPP_ASSERT_HTTP(result->isSuccess(), Status::CODE_500, result->getErrorMessage());
        OATPP_ASSERT_HTTP(!result->hasMoreToFetch(), Status::CODE_409, "USERNAME_ALREADY_EXISTS");
    }

    auto model = signUpDtoToUserModel(dto);

    auto res = database_->createOneUser(model);
    OATPP_ASSERT_HTTP(res->isSuccess(), Status::CODE_500, res->getErrorMessage());

    oatpp::Object<server::dto::SignInDto> signInDto = std::make_shared<dto::SignInDto>();
    signInDto->password = dto->password;
    signInDto->login = dto->email;

    return signIn(signInDto);
}

oatpp::Object<server::dto::AuthDto> AuthService::signIn(oatpp::Object<server::dto::SignInDto> const& dto) {

    std::shared_ptr<oatpp::orm::QueryResult> result;
    if (dto->login->find('@') != std::string::npos) {
        result = database_->getUserByEmail(dto->login);
    } else {
      result = database_->getUserByUsername(dto->login);
    }

    OATPP_ASSERT_HTTP(result->isSuccess(), Status::CODE_500, result->getErrorMessage());
    OATPP_ASSERT_HTTP(result->hasMoreToFetch(), Status::CODE_404, "USER_NOT_FOUND");

    auto fetch = result->fetch<oatpp::Vector<oatpp::Object<database::model::UserModel>>>();

    OATPP_ASSERT_HTTP(fetch->size() == 1, Status::CODE_500, "UNEXPECTED_NUMBER_OF_ROWS_FETCHED");

    OATPP_ASSERT_HTTP(dto->password == fetch[0]->password, Status::CODE_401, "WRONG_CREDENTIALS");

    auto payload = std::make_shared<auth::JWT::Payload>();
    payload->userId =  fetch[0]->id;
    payload->isAdmin = fetch[0]->admin;

    auto auth = server::dto::AuthDto::createShared();
    auth->tokenType = "bearer";
    auth->token = jwt_->createToken(payload);
    auth->createdAt = duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    auth->expiresAt = auth->createdAt + 2592000000;

    return auth;
}

}
