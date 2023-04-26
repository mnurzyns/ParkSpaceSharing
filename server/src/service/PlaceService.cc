#include "PlaceService.hh"

namespace server::service {

    oatpp::Object<server::dto::PlaceDto>
    PlaceService::createOne(
            oatpp::Object<server::dto::PlaceDto> const &dto
    ) {
        try {
            this->getOne(dto->id); // Will throw 404 if not found
            OATPP_ASSERT_HTTP(false, Status::CODE_409, "Place already exists")
        }
        catch (oatpp::web::protocol::http::HttpError &e) {
            if (e.getInfo().status != Status::CODE_404) { throw; }
        }

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
        OATPP_ASSERT_HTTP(queryResult->hasMoreToFetch(), Status::CODE_404, "Place not found")

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

        OATPP_ASSERT_HTTP(fetchTotalResult[0][0] > 0, Status::CODE_404, "No places found")

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
        auto existing = this->getOne(id);

        existing->address = dto->address ? dto->address : existing->address;
        existing->id = dto->id ? dto->id : existing->id;
        existing->latitude = dto->latitude ? dto->latitude : existing->latitude;
        existing->longitude = dto->longitude ? dto->longitude : existing->longitude;
        existing->ownerId = dto->ownerId ? dto->ownerId : existing->ownerId;

        return this->putOne(existing);
    }

    oatpp::Object<dto::StatusDto>
    PlaceService::deleteOne(
            const oatpp::UInt64 &id
    ) {
        this->getOne(id); // Will throw 404 if not found

        auto queryResult = database_->deletePlace(id);

        OATPP_ASSERT_HTTP(queryResult->isSuccess(), Status::CODE_500, queryResult->getErrorMessage())

        auto status = dto::StatusDto::createShared();
        status->status = "OK";
        status->code = Status::CODE_200.code;
        status->message = "Place was successfully deleted";
        return status;
    }

}  // namespace server::service
