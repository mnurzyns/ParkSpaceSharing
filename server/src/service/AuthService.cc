#include "AuthService.hh"

#include "EmailValidation.hh"
#include "Tel_numValidation.hh"

namespace server::service {

Object<StatusDto>
AuthService::signUp(Object<SignUpDto> const& dto)
{
    validateEmailHTTP(dto->email);
    validateTel_numHTTP(dto->tel_num);
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
        auto query_result = database_->getUserByTel_num(dto->tel_num);

        OATPP_ASSERT_HTTP(query_result->isSuccess(),
                          Status::CODE_500,
                          query_result->getErrorMessage())

        OATPP_ASSERT_HTTP(!query_result->hasMoreToFetch(),
                          Status::CODE_409,
                          "User with provided telephone number already exists")
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
    user_dto->tel_num = dto->tel_num;
    user_dto->password = Botan::argon2_generate_pwhash(
      dto->password->c_str(),
      dto->password->size(),
      Botan::system_rng(),
      1,
      65536, // TODO(papaj-na-wrotkach): move to config
      1);
    user_dto->role = user_dto->role = Role::User;

    auto query_result = database_->createUser(user_dto);

    OATPP_ASSERT_HTTP(query_result->isSuccess(),
                      Status::CODE_500,
                      query_result->getErrorMessage())

    auto status_dto = Object<StatusDto>::createShared();

    status_dto->code = Status::CODE_201.code;
    status_dto->message = "User created successfully";
    status_dto->status = "Created";

    return status_dto;
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

    OATPP_ASSERT_HTTP(Botan::argon2_check_pwhash(dto->password->c_str(),
                                                 dto->password->size(),
                                                 fetch[0]->password),
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
