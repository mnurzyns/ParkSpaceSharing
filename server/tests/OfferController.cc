#include "OfferController.hh"

#include "ApiClient.hh"
#include "Assert.hh"
#include "PlaceController.hh"
#include "dto/OfferDto.hh"
#include <oatpp/core/Types.hpp>

namespace tests {

void
offerPostTest(TestEnvironment const& env, AuthContext const& auth)
{
    OATPP_LOGD("[OfferController][POST][200]", "Valid request");
    deferFailure([&] {
        auto place = createDummyPlace(env, auth);

        auto dto = server::dto::OfferDto::createShared();
        dto->place_id = place->id;
        dto->date_from = 1;
        dto->date_to = 2;
        dto->description = "Cozy parking space";
        dto->price = 10;

        auto res = env.client->offerPost(auth.token, dto);
        testAssert(res->getStatusCode() == 200, assertWrap(res));

        auto returned =
          res->readBodyToDto<oatpp::Object<server::dto::OfferDto>>(env.mapper);
        testAssert(returned->id != nullptr &&
                   returned->place_id == dto->place_id &&
                   returned->date_from == dto->date_from &&
                   returned->date_to == dto->date_to &&
                   returned->description == dto->description &&
                   returned->price == dto->price);
    });

    OATPP_LOGD("[OfferController][POST][400]",
               "Invalid request - missing field");
    deferFailure([&] {
        auto place = createDummyPlace(env, auth);

        auto dto = server::dto::OfferDto::createShared();
        dto->place_id = place->id;
        dto->date_from = 1;
        dto->date_to = 2;
        dto->description = nullptr;
        dto->price = 10;

        auto res = env.client->offerPost(auth.token, dto);
        testAssert(res->getStatusCode() == 400, assertWrap(res));
    });

    OATPP_LOGD("[OfferController][POST][400]",
               "Invalid request - date_from > date_to");
    deferFailure([&] {
        auto place = createDummyPlace(env, auth);

        auto dto = server::dto::OfferDto::createShared();
        dto->place_id = place->id;
        dto->date_from = 2;
        dto->date_to = 1;
        dto->description = "Cozy parking space";
        dto->price = 10;

        auto res = env.client->offerPost(auth.token, dto);
        testAssert(res->getStatusCode() == 400, assertWrap(res));
    });

    OATPP_LOGD("[OfferController][POST][401]", "Unauthorized");
    deferFailure([&] {
        auto place = createDummyPlace(env, auth);

        auto dto = server::dto::OfferDto::createShared();
        dto->place_id = place->id;
        dto->date_from = 1;
        dto->date_to = 2;
        dto->description = "Cozy parking space";
        dto->price = 10;

        auto res = env.client->offerPost("", dto);
        testAssert(res->getStatusCode() == 401, assertWrap(res));
    });

    OATPP_LOGD("[OfferController][POST][403]", "Forbidden");
    deferFailure([&] {
        auto dto = server::dto::OfferDto::createShared();
        // Place id=1 created by migrations.
        dto->place_id = 1;
        dto->date_from = 1;
        dto->date_to = 2;
        dto->description = "Cozy parking space";
        dto->price = 10;

        auto res = env.client->offerPost(auth.token, dto);
        testAssert(res->getStatusCode() == 403, assertWrap(res));
    });

    OATPP_LOGD("[OfferController][POST][404]", "Not found");
    deferFailure([&] {
        auto place = createDummyPlace(env, auth);

        auto dto = server::dto::OfferDto::createShared();
        dto->place_id = 177013;
        dto->date_from = 1;
        dto->date_to = 2;
        dto->description = "Cozy parking space";
        dto->price = 10;

        auto res = env.client->offerPost(auth.token, dto);
        testAssert(res->getStatusCode() == 404, assertWrap(res));
    });

    OATPP_LOGD("[OfferController][POST][409]", "Conflict - id");
    deferFailure([&] {
        auto place = createDummyPlace(env, auth);

        auto dto = server::dto::OfferDto::createShared();
        // Offer id=1 created by migrations.
        dto->id = 1;
        dto->place_id = place->id;
        dto->date_from = 1;
        dto->date_to = 2;
        dto->description = "Cozy parking space";
        dto->price = 10;

        auto res = env.client->offerPost(auth.token, dto);
        testAssert(res->getStatusCode() == 409, assertWrap(res));
    });

    // https://github.com/mnurzyns/ParkSpaceSharing/issues/46

    // OATPP_LOGD("[OfferController][POST][409]",
    //            "Conflict - overlapping dates on same place_id");
    // deferFailure([&] {
    //     auto place = createDummyPlace(env, auth);

    //     auto dto = server::dto::OfferDto::createShared();
    //     dto->place_id = place->id;
    //     dto->date_from = 1;
    //     dto->date_to = 2;
    //     dto->description = "Cozy parking space";
    //     dto->price = 10;

    //     auto res1 = env.client->offerPost(auth.token, dto);
    //     testAssert(res1->getStatusCode() == 200, assertWrap(res1));

    //     auto res2 = env.client->offerPost(auth.token, dto);
    //     testAssert(res2->getStatusCode() == 409, assertWrap(res2));
    // });
}

oatpp::Object<server::dto::OfferDto>
createDummyOffer(TestEnvironment const& env, AuthContext const& auth)
{
    auto place = createDummyPlace(env, auth);

    auto dto = server::dto::OfferDto::createShared();
    dto->place_id = place->id;
    dto->date_from = 1;
    dto->date_to = 2;
    dto->description = "Cozy parking space";
    dto->price = 10;

    auto res = env.client->offerPost(auth.token, dto);
    testAssert(res->getStatusCode() == 200, assertWrap(res));

    return res->readBodyToDto<oatpp::Object<server::dto::OfferDto>>(env.mapper);
}

void
offerGetByIdTest(TestEnvironment const& env, AuthContext const& auth)
{
    OATPP_LOGD("[OfferController][GET][200]", "Valid request");
    deferFailure([&] {
        auto offer = createDummyOffer(env, auth);

        auto res = env.client->offerGetById(offer->id);
        testAssert(res->getStatusCode() == 200, assertWrap(res));

        auto returned =
          res->readBodyToDto<oatpp::Object<server::dto::OfferDto>>(env.mapper);
        testAssert(returned->id == offer->id &&
                   returned->place_id == offer->place_id &&
                   returned->date_from == offer->date_from &&
                   returned->date_to == offer->date_to &&
                   returned->description == offer->description &&
                   returned->price == offer->price);
    });

    OATPP_LOGD("[OfferController][GET][404]", "Not found");
    deferFailure([&] {
        auto res = env.client->offerGetById(177013);
        testAssert(res->getStatusCode() == 404, assertWrap(res));
    });
}

void
offerPatchTest(TestEnvironment const& env, AuthContext const& auth)
{
    OATPP_LOGD("[OfferController][PATCH][200]", "Valid request");
    deferFailure([&] {
        auto offer = createDummyOffer(env, auth);
        auto patch = server::dto::OfferDto::createShared();
        patch->date_to = offer->date_to + 10;

        auto res = env.client->offerPatch(auth.token, offer->id, patch);
        testAssert(res->getStatusCode() == 200, assertWrap(res));

        auto returned =
          res->readBodyToDto<oatpp::Object<server::dto::OfferDto>>(env.mapper);
        testAssert(returned->id == offer->id &&
                   returned->place_id == offer->place_id &&
                   returned->date_from == offer->date_from &&
                   returned->date_to == offer->date_to + 10 &&
                   returned->description == offer->description &&
                   returned->price == offer->price);
    });

    OATPP_LOGD("[OfferController][PATCH][200]",
               "Valid request - change place_id");
    deferFailure([&] {
        auto offer = createDummyOffer(env, auth);
        auto place = createDummyPlace(env, auth);
        auto patch = server::dto::OfferDto::createShared();
        patch->place_id = place->id;

        auto res = env.client->offerPatch(auth.token, offer->id, patch);
        testAssert(res->getStatusCode() == 200, assertWrap(res));

        auto returned =
          res->readBodyToDto<oatpp::Object<server::dto::OfferDto>>(env.mapper);
        testAssert(returned->id == offer->id &&
                   returned->place_id == place->id &&
                   returned->date_from == offer->date_from &&
                   returned->date_to == offer->date_to &&
                   returned->description == offer->description &&
                   returned->price == offer->price);
    });

    OATPP_LOGD("[OfferController][PATCH][400]", "Bad request - invalid date");
    deferFailure([&] {
        auto offer = createDummyOffer(env, auth);
        auto patch = server::dto::OfferDto::createShared();
        patch->date_to = 2;
        patch->date_from = 3;

        auto res = env.client->offerPatch(auth.token, offer->id, patch);
        testAssert(res->getStatusCode() == 400, assertWrap(res));
    });

    OATPP_LOGD("[OfferController][PATCH][401]", "Unauthorized");
    deferFailure([&] {
        auto offer = createDummyOffer(env, auth);
        auto patch = server::dto::OfferDto::createShared();

        auto res = env.client->offerPatch("", offer->id, patch);
        testAssert(res->getStatusCode() == 401, assertWrap(res));
    });

    OATPP_LOGD("[OfferController][PATCH][403]", "Forbidden - place_id");
    deferFailure([&] {
        auto offer = createDummyOffer(env, auth);
        auto patch = server::dto::OfferDto::createShared();
        // Place id=1 created by migrations.
        patch->place_id = 1;

        auto res = env.client->offerPatch(auth.token, offer->id, patch);
        testAssert(res->getStatusCode() == 403, assertWrap(res));
    });

    OATPP_LOGD("[OfferController][PATCH][404]", "Not found");
    deferFailure([&] {
        auto patch = server::dto::OfferDto::createShared();

        auto res = env.client->offerPatch(auth.token, 177013, patch);
        testAssert(res->getStatusCode() == 404, assertWrap(res));
    });

    OATPP_LOGD("[OfferController][PATCH][409]", "Conflict - id");
    deferFailure([&] {
        auto offer = createDummyOffer(env, auth);
        auto patch = server::dto::OfferDto::createShared();
        // Offer id=1 created by migrations.
        patch->id = 1;

        auto res = env.client->offerPatch(auth.token, offer->id, patch);
        testAssert(res->getStatusCode() == 409, assertWrap(res));
    });

    // https://github.com/mnurzyns/ParkSpaceSharing/issues/46

    // OATPP_LOGD("[OfferController][PATCH][409]", "Conflict");
    // deferFailure([&] {
    //     auto place = createDummyPlace(env, auth);
    //     auto dto = server::dto::OfferDto::createShared();
    //     dto->place_id = place->id;
    //     dto->date_from = 1;
    //     dto->date_to = 2;
    //     dto->description = "Cozy parking space";
    //     dto->price = 10;

    //     auto res1 = env.client->offerPost(auth.token, dto);
    //     testAssert(res1->getStatusCode() == 200, assertWrap(res1));

    //     auto offer = createDummyOffer(env, auth);
    //     auto patch = server::dto::OfferDto::createShared();
    //     patch->place_id = place->id;
    //     patch->date_from = 1;
    //     patch->date_to = 2;

    //     auto res2 = env.client->offerPatch(auth.token, offer->id, patch);
    //     testAssert(res2->getStatusCode() == 409, assertWrap(res2));
    // });
}

void
offerPutTest(TestEnvironment const& env, AuthContext const& auth)
{
    OATPP_LOGD("[OfferController][PUT][200]", "Valid request");
    deferFailure([&] {
        auto offer = createDummyOffer(env, auth);

        auto place = createDummyPlace(env, auth);
        offer->place_id = place->id;
        offer->date_from = 1;
        offer->date_to = 2;
        offer->description = "Different description";
        offer->price = 1;

        auto res = env.client->offerPut(auth.token, offer);
        testAssert(res->getStatusCode() == 200, assertWrap(res));

        auto returned =
          res->readBodyToDto<oatpp::Object<server::dto::OfferDto>>(env.mapper);
        testAssert(returned->id == offer->id &&
                   returned->place_id == offer->place_id &&
                   returned->date_from == offer->date_from &&
                   returned->date_to == offer->date_to &&
                   returned->description == offer->description &&
                   returned->price == offer->price);
    });

    OATPP_LOGD("[OfferController][PUT][201]", "Valid request");
    deferFailure([&] {
        auto offer = server::dto::OfferDto::createShared();
        auto place = createDummyPlace(env, auth);
        offer->id = 787878;
        offer->place_id = place->id;
        offer->date_from = 1;
        offer->date_to = 2;
        offer->description = "Different description";
        offer->price = 1;

        auto res = env.client->offerPut(auth.token, offer);
        testAssert(res->getStatusCode() == 201, assertWrap(res));

        auto returned =
          res->readBodyToDto<oatpp::Object<server::dto::OfferDto>>(env.mapper);
        testAssert(returned->id == offer->id &&
                   returned->place_id == offer->place_id &&
                   returned->date_from == offer->date_from &&
                   returned->date_to == offer->date_to &&
                   returned->description == offer->description &&
                   returned->price == offer->price);
    });

    OATPP_LOGD("[OfferController][PUT][400]", "Bad request - missing field");
    deferFailure([&] {
        auto offer = createDummyOffer(env, auth);

        auto place = createDummyPlace(env, auth);
        offer->place_id = place->id;
        offer->date_from = 1;
        offer->date_to = 2;
        offer->description = nullptr;
        offer->price = 1;

        auto res = env.client->offerPut(auth.token, offer);
        testAssert(res->getStatusCode() == 400, assertWrap(res));
    });

    OATPP_LOGD("[OfferController][PUT][400]",
               "Bad request - date_from > date_to");
    deferFailure([&] {
        auto offer = createDummyOffer(env, auth);
        offer->date_from = 2;
        offer->date_to = 1;

        auto res = env.client->offerPut(auth.token, offer);
        testAssert(res->getStatusCode() == 400, assertWrap(res));
        ;
    });

    OATPP_LOGD("[OfferController][PUT][401]", "Unauthorized");
    deferFailure([&] {
        auto offer = createDummyOffer(env, auth);

        auto res = env.client->offerPut("", offer);
        testAssert(res->getStatusCode() == 401, assertWrap(res));
    });

    OATPP_LOGD("[OfferController][PUT][403]", "Forbidden");
    deferFailure([&] {
        auto offer = createDummyOffer(env, auth);
        // Place id=1 created by migrations.
        offer->place_id = 1;

        auto res = env.client->offerPut(auth.token, offer);
        testAssert(res->getStatusCode() == 403, assertWrap(res));
    });

    OATPP_LOGD("[OfferController][PUT][404]", "Not found");
    deferFailure([&] {
        auto offer = createDummyOffer(env, auth);
        offer->place_id = 177013;

        auto res = env.client->offerPut(auth.token, offer);
        testAssert(res->getStatusCode() == 404, assertWrap(res));
    });

    // https://github.com/mnurzyns/ParkSpaceSharing/issues/46

    // OATPP_LOGD("[OfferController][PUT][409]", "Conflict");
    // deferFailure([&] {
    //     auto offer1 = createDummyOffer(env, auth);
    //     auto offer2 = createDummyOffer(env, auth);
    //     offer2->place_id = offer1->place_id;

    //     auto res = env.client->offerPut(auth.token, offer1);
    //     testAssert(res->getStatusCode() == 409, assertWrap(res));
    // });
}

void
offerDeleteTest(TestEnvironment const& env, AuthContext const& auth)
{
    OATPP_LOGD("[OfferController][DELETE][200]", "Valid request");
    deferFailure([&] {
        auto offer = createDummyOffer(env, auth);

        auto res1 = env.client->offerDelete(auth.token, offer->id);
        testAssert(res1->getStatusCode() == 200, assertWrap(res1));

        auto res2 = env.client->offerGetById(offer->id);
        testAssert(res2->getStatusCode() == 404, assertWrap(res2));
    });

    OATPP_LOGD("[OfferController][DELETE][401]", "Unauthorized");
    deferFailure([&] {
        auto offer = createDummyOffer(env, auth);

        auto res = env.client->offerDelete("", offer->id);
        testAssert(res->getStatusCode() == 401, assertWrap(res));
    });

    OATPP_LOGD("[OfferController][DELETE][403]", "Forbidden");
    deferFailure([&] {
        // Offer id=1 created by migrations.
        auto res = env.client->offerDelete(auth.token, 1);
        testAssert(res->getStatusCode() == 403, assertWrap(res));
    });

    OATPP_LOGD("[OfferController][DELETE][404]", "Not found");
    deferFailure([&] {
        auto res = env.client->offerDelete(auth.token, 177013);
        testAssert(res->getStatusCode() == 404, assertWrap(res));
    });
}

} // namespace tests
