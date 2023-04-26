#include "PlaceService.hh"

namespace server::service {

    oatpp::Object<server::dto::PlaceDto>
    PlaceService::createOne(
            oatpp::Object<server::dto::PlaceDto> const &dto
    ) {
        auto queryResult = database_->createPlace(dto);

        OATPP_ASSERT_HTTP(queryResult->isSuccess(), Status::CODE_500, queryResult->getErrorMessage())
        OATPP_ASSERT_HTTP(queryResult->hasMoreToFetch(), Status::CODE_500, "No rows returned!")

        auto fetchResult = queryResult->fetch<oatpp::Vector<oatpp::Object<dto::PlaceDto>>>();

        OATPP_ASSERT_HTTP(fetchResult->size() == 1, Status::CODE_500, "Unexpected number of rows returned!")

        return fetchResult[0];
    }

    oatpp::Object<server::dto::PlaceDto>
    PlaceService::getOne(
            oatpp::UInt64 const &id
    ) {
        auto queryResult = database_->getPlace(id);

        OATPP_ASSERT_HTTP(queryResult->isSuccess(), Status::CODE_500, queryResult->getErrorMessage())
        OATPP_ASSERT_HTTP(queryResult->hasMoreToFetch(), Status::CODE_404, "Not found")

        auto fetchResult = queryResult->fetch<oatpp::Vector<oatpp::Object<dto::PlaceDto>>>();

        OATPP_ASSERT_HTTP(fetchResult->size() == 1, Status::CODE_500, "Unexpected number of rows returned!")

        return fetchResult[0];
    }

    oatpp::Object<dto::PlacePageDto>
    PlaceService::search(
            oatpp::String const &query,
            oatpp::UInt64 const &limit,
            oatpp::UInt64 const &offset
    ) {
        std::string const queryTableFts = " FROM place_fts";
        std::string const queryFilters = query->empty() ? std::string{} : " WHERE place_fts MATCH :query";

        auto queryTotalResult = database_->executeQuery(
                "SELECT COUNT(*)" + queryTableFts + queryFilters + ";",
                {{"query", oatpp::String(query)}}
        );

        OATPP_ASSERT_HTTP(queryTotalResult->isSuccess(), Status::CODE_500, queryTotalResult->getErrorMessage())

        auto fetchTotalResult = queryTotalResult->fetch<oatpp::Vector<oatpp::Vector<oatpp::UInt64>>>();

        OATPP_ASSERT_HTTP(fetchTotalResult[0][0] > 0, Status::CODE_404, "Not found")

        auto queryResult = database_->executeQuery(
                "SELECT place.*" + queryTableFts +
                " INNER JOIN place ON place.id = place_fts.place_id" + queryFilters +
                " LIMIT :offset,:limit;", {
                        {"query",  oatpp::String(query)},
                        {"offset", oatpp::UInt64(offset)},
                        {"limit",  oatpp::UInt64(limit)}
                });

        auto fetchResult = queryResult->fetch<oatpp::Vector<oatpp::Object<dto::PlaceDto>>>();

        auto page = dto::PlacePageDto::createShared();
        page->items = fetchResult;
        page->limit = limit;
        page->offset = offset;
        page->count = fetchTotalResult[0][0];

        return page;
    }

    oatpp::Object<dto::PlaceDto>
    PlaceService::putOne(
            oatpp::Object<dto::PlaceDto> const &dto
    ) {
        auto queryResult = database_->replacePlace(dto);

        OATPP_ASSERT_HTTP(queryResult->isSuccess(), Status::CODE_500, queryResult->getErrorMessage())
        OATPP_ASSERT_HTTP(queryResult->hasMoreToFetch(), Status::CODE_500, "No rows returned!")

        auto fetchResult = queryResult->fetch<oatpp::Vector<oatpp::Object<dto::PlaceDto>>>();

        OATPP_ASSERT_HTTP(fetchResult->size() == 1, Status::CODE_500, "Unexpected number of rows returned!")

        return fetchResult[0];
    }

    oatpp::Object<dto::PlaceDto>
    PlaceService::patchOne(
            oatpp::UInt64 const &id,
            oatpp::Object<dto::PlaceDto> const &dto
    ) {
        auto queryResult = database_->getPlace(id);

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

        queryResult = database_->replacePlace(existing);

        OATPP_ASSERT_HTTP(queryResult->isSuccess(), Status::CODE_500, queryResult->getErrorMessage())
        OATPP_ASSERT_HTTP(queryResult->hasMoreToFetch(), Status::CODE_500, "No rows returned!")

        fetchResult = queryResult->fetch<oatpp::Vector<oatpp::Object<dto::PlaceDto>>>();

        OATPP_ASSERT_HTTP(fetchResult->size() == 1, Status::CODE_500, "Unexpected number of rows returned!")

        return fetchResult[0];
    }

    oatpp::Object<dto::StatusDto>
    PlaceService::deleteOne(
            const oatpp::UInt64 &id
    ) {
        auto queryResult = database_->getPlace(id);

        OATPP_ASSERT_HTTP(queryResult->isSuccess(), Status::CODE_500, queryResult->getErrorMessage())
        OATPP_ASSERT_HTTP(queryResult->hasMoreToFetch(), Status::CODE_404, "Not found")

        queryResult = database_->deletePlace(id);

        OATPP_ASSERT_HTTP(queryResult->isSuccess(), Status::CODE_500, queryResult->getErrorMessage())

        auto status = dto::StatusDto::createShared();
        status->status = "OK";
        status->code = Status::CODE_200.code;
        status->message = "User story was successfully deleted";
        return status;
    }

}  // namespace server::service
