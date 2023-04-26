#include "AuthService.hh"

namespace server::service {

    oatpp::Object<server::dto::AuthDto> AuthService::signUp(
            oatpp::Object<server::dto::SignUpDto> const &dto
    ) {
        OATPP_ASSERT_HTTP(
                !dto->email->empty() && !dto->password->empty() && !dto->username->empty(),
                Status::CODE_400,
                "REQUIRED_PARAMETER_NOT_PROVIDED"
        )
        {
            auto result = database_->getUserByEmail(dto->email);
            OATPP_ASSERT_HTTP(result->isSuccess(), Status::CODE_500, result->getErrorMessage())
            OATPP_ASSERT_HTTP(!result->hasMoreToFetch(), Status::CODE_409, "EMAIL_ALREADY_EXISTS")
        }
        {
            auto result = database_->getUserByUsername(dto->username);
            OATPP_ASSERT_HTTP(result->isSuccess(), Status::CODE_500, result->getErrorMessage())
            OATPP_ASSERT_HTTP(!result->hasMoreToFetch(), Status::CODE_409, "USERNAME_ALREADY_EXISTS")
        }

        auto userDto = oatpp::Object<dto::UserDto>::createShared();
        userDto->email = dto->email;
        userDto->username = dto->username;
        userDto->password = dto->password;
        userDto->role = 1;

        auto res = database_->createUser(userDto);
        OATPP_ASSERT_HTTP(res->isSuccess(), Status::CODE_500, res->getErrorMessage())

        oatpp::Object<server::dto::SignInDto> const signInDto = std::make_shared<dto::SignInDto>();
        signInDto->login = dto->email;
        signInDto->password = dto->password;

        return signIn(signInDto);
    }

    oatpp::Object<server::dto::AuthDto> AuthService::signIn(oatpp::Object<server::dto::SignInDto> const &dto) {
        OATPP_ASSERT_HTTP(
                !dto->login->empty() && !dto->password->empty(),
                Status::CODE_400,
                "REQUIRED_PARAMETER_NOT_PROVIDED"
        )

        std::shared_ptr<oatpp::orm::QueryResult> result;
        if (dto->login->find('@') != std::string::npos) {
            result = database_->getUserByEmail(dto->login);
        } else {
            result = database_->getUserByUsername(dto->login);
        }

        OATPP_ASSERT_HTTP(result->isSuccess(), Status::CODE_500, result->getErrorMessage())
        OATPP_ASSERT_HTTP(result->hasMoreToFetch(), Status::CODE_404, "NOT_FOUND")

        auto fetch = result->fetch<oatpp::Vector<oatpp::Object<dto::UserDto>>>();

        OATPP_ASSERT_HTTP(fetch->size() == 1, Status::CODE_500, "UNEXPECTED_NUMBER_OF_ROWS_FETCHED")

        OATPP_ASSERT_HTTP(dto->password == fetch[0]->password, Status::CODE_401, "WRONG_CREDENTIALS")

        auto payload = std::make_shared<auth::JWT::Payload>();
        payload->userId = fetch[0]->id;
        payload->role = fetch[0]->role;

        auto auth = server::dto::AuthDto::createShared();
        auth->tokenType = "bearer";
        auth->token = jwt_->createToken(payload);
        auth->createdAt = duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch()).count();
        auth->expiresAt = auth->createdAt + 2592000000; //TODO: move to config or something

        return auth;
    }

}  // namespace server::service
