#include "PlaceService.hh"

namespace server::service {

    oatpp::Object<server::dto::PlaceDto>
    PlaceService::createOne(
            oatpp::Object<server::dto::PlaceDto> const &dto
    ) {
        auto queryResult = database->createPlace(dto);

        OATPP_ASSERT_HTTP(queryResult->isSuccess(), Status::CODE_500, queryResult->getErrorMessage())
        OATPP_ASSERT_HTTP(queryResult->hasMoreToFetch(), Status::CODE_500, "No rows returned!")

        auto fetchResult = queryResult->fetch<oatpp::Vector<oatpp::Object<dto::PlaceDto>>>();

        OATPP_ASSERT_HTTP(fetchResult->size() == 1, Status::CODE_500, "Unexpected number of rows returned!")

        return fetchResult[0];
    }

    oatpp::Object<server::dto::PlaceDto>
    PlaceService::getOne(
            oatpp::UInt64 const &placeId
    ) {
        auto queryResult = database->getPlace(placeId);

        OATPP_ASSERT_HTTP(queryResult->isSuccess(), Status::CODE_500, queryResult->getErrorMessage())
        OATPP_ASSERT_HTTP(queryResult->hasMoreToFetch(), Status::CODE_404, "Not found")

        auto fetchResult = queryResult->fetch<oatpp::Vector<oatpp::Object<dto::PlaceDto>>>();

        OATPP_ASSERT_HTTP(fetchResult->size() == 1, Status::CODE_500, "Unexpected number of rows returned!")

        return fetchResult[0];
    }

    oatpp::Object<dto::PlaceDto>
    PlaceService::putOne(
            oatpp::Object<dto::PlaceDto> const &dto
    ) {
        auto queryResult = database->replacePlace(dto);

        OATPP_ASSERT_HTTP(queryResult->isSuccess(), Status::CODE_500, queryResult->getErrorMessage())
        OATPP_ASSERT_HTTP(queryResult->hasMoreToFetch(), Status::CODE_500, "No rows returned!")

        auto fetchResult = queryResult->fetch<oatpp::Vector<oatpp::Object<dto::PlaceDto>>>();

        OATPP_ASSERT_HTTP(fetchResult->size() == 1, Status::CODE_500, "Unexpected number of rows returned!")

        return fetchResult[0];
    }

    oatpp::Object<dto::PlaceDto>
    PlaceService::patchOne(
            oatpp::UInt64 const &placeId,
            oatpp::Object<dto::PlaceDto> const &dto
    ) {
        auto queryResult = database->getPlace(placeId);

        OATPP_ASSERT_HTTP(queryResult->isSuccess(), Status::CODE_500, queryResult->getErrorMessage())
        OATPP_ASSERT_HTTP(queryResult->hasMoreToFetch(), Status::CODE_500, "No rows returned!")

        auto fetchResult = queryResult->fetch<oatpp::Vector<oatpp::Object<dto::PlaceDto>>>();

        OATPP_ASSERT_HTTP(fetchResult->size() == 1, Status::CODE_500, "Unexpected number of rows returned!")

        auto existing = fetchResult[0];

        existing->address = dto->address ? dto->address : existing->address;
        existing->id = dto->id ? dto->id : existing->id;
        existing->latitude = dto->latitude ? dto->latitude : existing->latitude;
        existing->longitude = dto->longitude ? dto->longitude : existing->longitude;
        existing->ownerId = dto->ownerId ? dto->ownerId : existing->ownerId;

        queryResult = database->replacePlace(existing);

        OATPP_ASSERT_HTTP(queryResult->isSuccess(), Status::CODE_500, queryResult->getErrorMessage())
        OATPP_ASSERT_HTTP(queryResult->hasMoreToFetch(), Status::CODE_500, "No rows returned!")

        fetchResult = queryResult->fetch<oatpp::Vector<oatpp::Object<dto::PlaceDto>>>();

        OATPP_ASSERT_HTTP(fetchResult->size() == 1, Status::CODE_500, "Unexpected number of rows returned!")

        return fetchResult[0];
    }

    oatpp::Object<dto::StatusDto>
    PlaceService::deleteOne(
            const oatpp::UInt64 &placeId
    ) {
        auto queryResult = database->getPlace(placeId);

        OATPP_ASSERT_HTTP(queryResult->isSuccess(), Status::CODE_500, queryResult->getErrorMessage())
        OATPP_ASSERT_HTTP(queryResult->hasMoreToFetch(), Status::CODE_404, "Not found")

        queryResult = database->deletePlace(placeId);

        OATPP_ASSERT_HTTP(queryResult->isSuccess(), Status::CODE_500, queryResult->getErrorMessage())

        auto status = dto::StatusDto::createShared();
        status->status = "OK";
        status->code = Status::CODE_200.code;
        status->message = "User story was successfully deleted";
        return status;
    }

}  // namespace server::service
