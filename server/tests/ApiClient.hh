#pragma once

#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/web/client/ApiClient.hpp>

#include "dto/OfferDto.hh"
#include "dto/PlaceDto.hh"
#include "dto/SignInDto.hh"
#include "dto/SignUpDto.hh"
#include "dto/UserDto.hh"

namespace tests {

#include OATPP_CODEGEN_BEGIN(ApiClient)

class ApiClient : public oatpp::web::client::ApiClient
{
  public:
    API_CLIENT_INIT(ApiClient)

    // User
    API_CALL("POST",
             "signup",
             signup,
             BODY_DTO(oatpp::Object<server::dto::SignUpDto>, dto))
    API_CALL("POST",
             "signin",
             signin,
             BODY_DTO(oatpp::Object<server::dto::SignInDto>, dto))

    // Place
    API_CALL("POST",
             "place",
             place_create_one,
             AUTHORIZATION(String, auth_string, "Bearer"),
             BODY_DTO(oatpp::Object<server::dto::PlaceDto>, dto))
    API_CALL("PUT",
             "place",
             place_put_one,
             AUTHORIZATION(String, auth_string, "Bearer"),
             BODY_DTO(oatpp::Object<server::dto::PlaceDto>, dto))
    API_CALL("PATCH",
             "place/{id}",
             place_patch_one,
             AUTHORIZATION(String, auth_string, "Bearer"),
             PATH(UInt64, id),
             BODY_DTO(oatpp::Object<server::dto::PlaceDto>, dto))
    API_CALL("DELETE",
             "place/{id}",
             place_delete_one,
             AUTHORIZATION(String, auth_string, "Bearer"),
             PATH(UInt64, id))

    // Offer
    API_CALL("POST",
             "offer",
             offer_create_one,
             AUTHORIZATION(String, auth_string, "Bearer"),
             BODY_DTO(oatpp::Object<server::dto::OfferDto>, dto))
    API_CALL("DELETE",
             "offer/{id}",
             offer_delete_one,
             AUTHORIZATION(String, auth_string, "Bearer"),
             PATH(UInt64, id))
};

#include OATPP_CODEGEN_END(ApiClient)

} // namespace tests
