#include "AuthService.hh"
#include "TokenPayload.hh"

namespace server::service {

Object<AuthDto>
AuthService::signUp(Object<SignUpDto> const& dto)
{
    {
        auto query_result = database_->getUserByEmail(dto->email);

        OATPP_ASSERT_HTTP(query_result->isSuccess(),
                          Status::CODE_500,
                          query_result->getErrorMessage())

        OATPP_ASSERT_HTTP(!query_result->hasMoreToFetch(),
                          Status::CODE_409,
                          "User with provided email already exists")
    }
    {
        auto query_result = database_->getUserByUsername(dto->username);

        OATPP_ASSERT_HTTP(query_result->isSuccess(),
                          Status::CODE_500,
                          query_result->getErrorMessage())

        OATPP_ASSERT_HTTP(!query_result->hasMoreToFetch(),
                          Status::CODE_409,
                          "User with provided username already exists")
    }

    auto user_dto = Object<UserDto>::createShared();
    user_dto->email = dto->email;
    user_dto->username = dto->username;
    user_dto->password = dto->password;
    user_dto->role = Role::User;

    auto query_result = database_->createUser(user_dto);

    OATPP_ASSERT_HTTP(query_result->isSuccess(),
                      Status::CODE_500,
                      query_result->getErrorMessage())

    Object<SignInDto> const sign_in_dto = std::make_shared<SignInDto>();
    sign_in_dto->login = dto->email;
    sign_in_dto->password = dto->password;

    return signIn(sign_in_dto);
}

Object<AuthDto>
AuthService::signIn(Object<SignInDto> const& dto)
{
    std::shared_ptr<oatpp::orm::QueryResult> query_result;
    if (dto->login->find('@') != std::string::npos) {
        query_result = database_->getUserByEmail(dto->login);
    } else {
        query_result = database_->getUserByUsername(dto->login);
    }

    OATPP_ASSERT_HTTP(query_result->isSuccess(),
                      Status::CODE_500,
                      query_result->getErrorMessage())

    OATPP_ASSERT_HTTP(
      query_result->hasMoreToFetch(), Status::CODE_404, "User not found")

    auto fetch = query_result->fetch<Vector<Object<UserDto>>>();

    OATPP_ASSERT_HTTP(fetch->size() == 1,
                      Status::CODE_500,
                      "Unexpected number of rows returned!")

    OATPP_ASSERT_HTTP(dto->password == fetch[0]->password,
                      Status::CODE_401,
                      "Invalid credentials")

    auto payload = std::make_shared<TokenPayload>();
    payload->user_id = fetch[0]->id;
    payload->user_role = fetch[0]->role;

    auto auth_dto = AuthDto::createShared();
    auth_dto->token_type = "JWS";
    auth_dto->token = token_utils_component->createToken(payload);

    return auth_dto;
}

} // namespace server::service
