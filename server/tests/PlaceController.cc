#include "PlaceController.hh"

#include "ApiClient.hh"
#include "Assert.hh"
#include "dto/OfferDto.hh"
#include "dto/PlaceDto.hh"

namespace tests {

void
placePostTest(TestEnvironment const& env, AuthContext const& auth)
{
    OATPP_LOGD("[PlaceController][POST][200]", "Valid request");
    deferFailure([&] {
        auto dto = server::dto::PlaceDto::createShared();
        dto->owner_id = auth.token_payload.user_id;
        dto->address = "West street";
        dto->latitude = 25.31662036314199;
        dto->longitude = 51.46711279943629;

        auto res = env.client->placePost(auth.token, dto);
        testAssert(res->getStatusCode() == 200, assertWrap(res));

        auto returned =
          res->readBodyToDto<oatpp::Object<server::dto::PlaceDto>>(env.mapper);
        testAssert(returned->id != nullptr &&
                   returned->owner_id == dto->owner_id &&
                   returned->address == dto->address &&
                   returned->latitude == dto->latitude &&
                   returned->longitude == dto->longitude);
    });

    OATPP_LOGD("[PlaceController][POST][400]", "Bad request");
    deferFailure([&] {
        auto dto = server::dto::PlaceDto::createShared();
        dto->owner_id = auth.token_payload.user_id;
        dto->address = nullptr;
        dto->latitude = 25.31662036314199;
        dto->longitude = 51.46711279943629;

        auto res = env.client->placePost(auth.token, dto);
        testAssert(res->getStatusCode() == 400, assertWrap(res));
    });

    OATPP_LOGD("[PlaceController][POST][401]", "Unauthorized");
    deferFailure([&] {
        auto dto = server::dto::PlaceDto::createShared();
        dto->owner_id = auth.token_payload.user_id;
        dto->address = "West street";
        dto->latitude = 25.31662036314199;
        dto->longitude = 51.46711279943629;

        auto res = env.client->placePost("", dto);
        testAssert(res->getStatusCode() == 401, assertWrap(res));
    });

    OATPP_LOGD("[PlaceController][POST][403]", "Forbidden");
    deferFailure([&] {
        auto dto = server::dto::PlaceDto::createShared();
        dto->owner_id = 1; // Created by migrations
        dto->address = "West street";
        dto->latitude = 25.31662036314199;
        dto->longitude = 51.46711279943629;

        auto res = env.client->placePost(auth.token, dto);
        testAssert(res->getStatusCode() == 403, assertWrap(res));
    });

    OATPP_LOGD("[PlaceController][POST][409]", "Conflict");
    deferFailure([&] {
        auto dto = server::dto::PlaceDto::createShared();
        dto->id = 1;
        dto->owner_id = auth.token_payload.user_id;
        dto->address = "West street";
        dto->latitude = 25.31662036314199;
        dto->longitude = 51.46711279943629;

        auto res = env.client->placePost(auth.token, dto);
        testAssert(res->getStatusCode() == 409, assertWrap(res));
    });
}

/*
  Requires working POST endpoint.
 */
oatpp::Object<server::dto::PlaceDto>
createDummyPlace(TestEnvironment const& env, AuthContext const& auth)
{
    // OATPP_LOGD("[PlaceController]", "Create dummy place");
    auto dto = server::dto::PlaceDto::createShared();
    dto->owner_id = auth.token_payload.user_id;
    dto->address = "Dummy place";
    dto->latitude = 25.31662036314199;
    dto->longitude = 51.46711279943629;

    auto res = env.client->placePost(auth.token, dto);
    testAssert(res->getStatusCode() == 200, assertWrap(res));
    return res->readBodyToDto<oatpp::Object<server::dto::PlaceDto>>(env.mapper);
}

void
placeGetByIdTest(TestEnvironment const& env, AuthContext const& auth)
{
    OATPP_LOGD("[PlaceController][GET][200]", "Valid request");
    deferFailure([&] {
        auto dto = createDummyPlace(env, auth);

        auto res = env.client->placeGetById(dto->id);
        testAssert(res->getStatusCode() == 200, assertWrap(res));

        auto returned =
          res->readBodyToDto<oatpp::Object<server::dto::PlaceDto>>(env.mapper);
        testAssert(returned->id == dto->id &&
                   returned->owner_id == dto->owner_id &&
                   returned->address == dto->address &&
                   returned->latitude == dto->latitude &&
                   returned->longitude == dto->longitude);
    });

    OATPP_LOGD("[PlaceController][GET][404]", "Not found");
    deferFailure([&] {
        auto res = env.client->placeGetById(177013);
        testAssert(res->getStatusCode() == 404, assertWrap(res));
    });
}

void
placePatchTest(TestEnvironment const& env, AuthContext const& auth)
{
    OATPP_LOGD("[PlaceController][PATCH][200]", "Valid request");
    deferFailure([&] {
        auto dto = createDummyPlace(env, auth);
        auto patch = server::dto::PlaceDto::createShared();
        patch->address = "East street";

        auto res = env.client->placePatch(auth.token, dto->id, patch);
        testAssert(res->getStatusCode() == 200, assertWrap(res));

        auto returned =
          res->readBodyToDto<oatpp::Object<server::dto::PlaceDto>>(env.mapper);
        testAssert(returned->id == dto->id &&
                   returned->owner_id == dto->owner_id &&
                   returned->address == "East street" &&
                   returned->latitude == dto->latitude &&
                   returned->longitude == dto->longitude);
    });

    OATPP_LOGD("[PlaceController][PATCH][401]", "Unauthorized");
    deferFailure([&] {
        auto dto = createDummyPlace(env, auth);
        auto patch = server::dto::PlaceDto::createShared();
        patch->address = "East street";

        auto res = env.client->placePatch("", dto->id, patch);
        testAssert(res->getStatusCode() == 401, assertWrap(res));
    });

    OATPP_LOGD("[PlaceController][PATCH][403]", "Forbidden - owner_id");
    deferFailure([&] {
        auto dto = createDummyPlace(env, auth);
        auto patch = server::dto::PlaceDto::createShared();
        // User id=1 created by migrations.
        patch->id = dto->id;
        patch->owner_id = 1;

        auto res = env.client->placePatch(auth.token, dto->id, patch);
        testAssert(res->getStatusCode() == 403, assertWrap(res));
    });

    OATPP_LOGD("[PlaceController][PATCH][404]", "Not found");
    deferFailure([&] {
        auto patch = server::dto::PlaceDto::createShared();

        auto res = env.client->placePatch(auth.token, 177013, patch);
        testAssert(res->getStatusCode() == 404, assertWrap(res));
    });

    OATPP_LOGD("[PlaceController][PATCH][409]", "Conflict - id");
    deferFailure([&] {
        auto dto = createDummyPlace(env, auth);
        auto patch = server::dto::PlaceDto::createShared();
        // Place id=1 created by migrations.
        patch->id = 1;

        auto res = env.client->placePatch(auth.token, dto->id, patch);
        testAssert(res->getStatusCode() == 409, assertWrap(res));
    });
}

void
placePutTest(TestEnvironment const& env, AuthContext const& auth)
{
    OATPP_LOGD("[PlaceController][PUT][200]", "Valid request");
    deferFailure([&] {
        auto dto = createDummyPlace(env, auth);
        dto->address = "East street";
        dto->latitude = 123.123;
        dto->longitude = 321.321;

        auto res = env.client->placePut(auth.token, dto);
        testAssert(res->getStatusCode() == 200, assertWrap(res));

        auto returned =
          res->readBodyToDto<oatpp::Object<server::dto::PlaceDto>>(env.mapper);
        testAssert(returned->id == dto->id &&
                   returned->owner_id == dto->owner_id &&
                   returned->address == dto->address &&
                   returned->latitude == dto->latitude &&
                   returned->longitude == dto->longitude);
    });

    OATPP_LOGD("[PlaceController][PUT][201]", "Valid request");
    deferFailure([&] {
        auto dto = server::dto::PlaceDto::createShared();
        dto->id = 989898;
        dto->owner_id = auth.token_payload.user_id;
        dto->address = "East street";
        dto->latitude = 123.123;
        dto->longitude = 321.321;

        auto res = env.client->placePut(auth.token, dto);
        testAssert(res->getStatusCode() == 201, assertWrap(res));

        auto returned =
          res->readBodyToDto<oatpp::Object<server::dto::PlaceDto>>(env.mapper);
        testAssert(returned->id == dto->id &&
                   returned->owner_id == dto->owner_id &&
                   returned->address == dto->address &&
                   returned->latitude == dto->latitude &&
                   returned->longitude == dto->longitude);
    });

    OATPP_LOGD("[PlaceController][PUT][400]", "Bad request");
    deferFailure([&] {
        auto dto = createDummyPlace(env, auth);
        dto->latitude = nullptr;

        auto res = env.client->placePut(auth.token, dto);
        testAssert(res->getStatusCode() == 400, assertWrap(res));
    });

    OATPP_LOGD("[PlaceController][PUT][401]", "Unauthorized");
    deferFailure([&] {
        auto dto = createDummyPlace(env, auth);

        auto res = env.client->placePut("", dto);
        testAssert(res->getStatusCode() == 401, assertWrap(res));
    });

    OATPP_LOGD("[PlaceController][PUT][403]", "Forbidden - id");
    deferFailure([&] {
        auto dto = createDummyPlace(env, auth);
        // Place id=1 created by migrations.
        dto->id = 1;

        auto res = env.client->placePut(auth.token, dto);
        testAssert(res->getStatusCode() == 403, assertWrap(res));
    });

    OATPP_LOGD("[PlaceController][PUT][403]", "Forbidden - owner_id");
    deferFailure([&] {
        auto dto = createDummyPlace(env, auth);
        // User id=1 created by migrations.
        dto->owner_id = 1;

        auto res = env.client->placePut(auth.token, dto);
        testAssert(res->getStatusCode() == 403, assertWrap(res));
    });
}

void
placeDeleteTest(TestEnvironment const& env, AuthContext const& auth)
{
    OATPP_LOGD("[PlaceController][DELETE][200]", "Valid request");
    deferFailure([&] {
        // This also checks if offers connected to this place were deleted.
        auto place = createDummyPlace(env, auth);
        auto offer = server::dto::OfferDto::createShared();
        offer->place_id = place->id;
        offer->date_from = 1;
        offer->date_to = 2;
        offer->description = "This should be deleted together with place";
        offer->price = 3;

        deferFailure([&] {
            auto res1 = env.client->offerPost(auth.token, offer);
            testAssert(res1->getStatusCode() == 200, assertWrap(res1));
            offer = res1->readBodyToDto<oatpp::Object<server::dto::OfferDto>>(env.mapper);
        });

        auto res2 = env.client->placeDelete(auth.token, place->id);
        testAssert(res2->getStatusCode() == 200, assertWrap(res2));

        auto res3 = env.client->placeGetById(place->id);
        testAssert(res3->getStatusCode() == 404, assertWrap(res3));

        deferFailure([&] {
            auto res4 = env.client->offerGetById(offer->id);
            testAssert(res4->getStatusCode() == 404, assertWrap(res4));
        });
    });

    OATPP_LOGD("[PlaceController][DELETE][401]", "Unauthorized");
    deferFailure([&] {
        auto place = createDummyPlace(env, auth);

        auto res = env.client->placeDelete("", place->id);
        testAssert(res->getStatusCode() == 401, assertWrap(res));
    });

    OATPP_LOGD("[PlaceController][DELETE][403]", "Unauthorized");
    deferFailure([&] {
        // Place id=1 created by migrations.
        auto res = env.client->placeDelete(auth.token, 1);
        testAssert(res->getStatusCode() == 403, assertWrap(res));
    });

    OATPP_LOGD("[PlaceController][DELETE][404]", "Not found");
    deferFailure([&] {
        auto res = env.client->placeDelete(auth.token, 177013);
        testAssert(res->getStatusCode() == 404, assertWrap(res));
    });
}

} // namespace tests
