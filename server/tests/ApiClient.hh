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
    API_CALL("GET",
             "place/{id}",
             placeGetById,
             PATH(UInt64, id))
    API_CALL("POST",
             "place",
             placePost,
             AUTHORIZATION(String, auth_string, "Bearer"),
             BODY_DTO(oatpp::Object<server::dto::PlaceDto>, dto))
    API_CALL("PUT",
             "place",
             placePut,
             AUTHORIZATION(String, auth_string, "Bearer"),
             BODY_DTO(oatpp::Object<server::dto::PlaceDto>, dto))
    API_CALL("PATCH",
             "place/{id}",
             placePatch,
             AUTHORIZATION(String, auth_string, "Bearer"),
             PATH(UInt64, id),
             BODY_DTO(oatpp::Object<server::dto::PlaceDto>, dto))
    API_CALL("DELETE",
             "place/{id}",
             placeDelete,
             AUTHORIZATION(String, auth_string, "Bearer"),
             PATH(UInt64, id))

    // Offer
    API_CALL("POST",
             "offer",
             offerPost,
             AUTHORIZATION(String, auth_string, "Bearer"),
             BODY_DTO(oatpp::Object<server::dto::OfferDto>, dto))
    API_CALL("DELETE",
             "offer/{id}",
             offerDelete,
             AUTHORIZATION(String, auth_string, "Bearer"),
             PATH(UInt64, id))
};

#include OATPP_CODEGEN_END(ApiClient)

} // namespace tests
