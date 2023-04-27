#include "AuthService.hh"

namespace server::service {

    oatpp::Object<server::dto::AuthDto> AuthService::signUp(
            oatpp::Object<server::dto::SignUpDto> const &dto
    ) {
        {
            auto result = database_->getUserByEmail(dto->email);
            OATPP_ASSERT_HTTP(
                    result->isSuccess(),
                    Status::CODE_500,
                    result->getErrorMessage()
            )
            OATPP_ASSERT_HTTP(
                    !result->hasMoreToFetch(),
                    Status::CODE_409,
                    "User with provided email already exists"
            )
        }
        {
            auto result = database_->getUserByUsername(dto->username);
            OATPP_ASSERT_HTTP(
                    result->isSuccess(),
                    Status::CODE_500,
                    result->getErrorMessage()
            )
            OATPP_ASSERT_HTTP(
                    !result->hasMoreToFetch(),
                    Status::CODE_409,
                    "User with provided username already exists"
            )
        }

        auto userDto = oatpp::Object<dto::UserDto>::createShared();
        userDto->email = dto->email;
        userDto->username = dto->username;
        userDto->password = dto->password;
        userDto->role = 1;

        auto queryResult = database_->createUser(userDto);
        OATPP_ASSERT_HTTP(
                queryResult->isSuccess(),
                Status::CODE_500,
                queryResult->getErrorMessage()
        )

        oatpp::Object<server::dto::SignInDto> const signInDto = std::make_shared<dto::SignInDto>();
        signInDto->login = dto->email;
        signInDto->password = dto->password;

        return signIn(signInDto);
    }

    oatpp::Object<server::dto::AuthDto> AuthService::signIn(oatpp::Object<server::dto::SignInDto> const &dto) {
        std::shared_ptr<oatpp::orm::QueryResult> queryResult;
        if (dto->login->find('@') != std::string::npos) {
            queryResult = database_->getUserByEmail(dto->login);
        } else {
            queryResult = database_->getUserByUsername(dto->login);
        }

        OATPP_ASSERT_HTTP(
                queryResult->isSuccess(),
                Status::CODE_500,
                queryResult->getErrorMessage()
        )
        OATPP_ASSERT_HTTP(
                queryResult->hasMoreToFetch(),
                Status::CODE_404,
                "User not found"
        )

        auto fetch = queryResult->fetch<oatpp::Vector<oatpp::Object<dto::UserDto>>>();

        OATPP_ASSERT_HTTP(
                fetch->size() == 1,
                Status::CODE_500,
                "Unexpected number of rows returned!"
        )

        OATPP_ASSERT_HTTP(
                dto->password == fetch[0]->password,
                Status::CODE_401,
                "Invalid credentials"
        )

        auto payload = std::make_shared<auth::JWT::Payload>();
        payload->userId = fetch[0]->id;
        payload->role = fetch[0]->role;

        auto authDto = server::dto::AuthDto::createShared();
        authDto->tokenType = "JWS";
        authDto->token = jwt_->createToken(payload);

        return authDto;
    }

}  // namespace server::service
