#include "OfferService.hh"

namespace server::service {

    oatpp::Object<server::dto::OfferDto>
    OfferService::createOne(
            oatpp::Object<server::dto::OfferDto> const &dto
    ) {
        try {
            this->getOne(dto->id); // Will throw 404 if not found
            OATPP_ASSERT_HTTP(false, Status::CODE_409, "Offer already exists")
        }
        catch (oatpp::web::protocol::http::HttpError &e) {
            if (e.getInfo().status != Status::CODE_404) { throw; }
        }

        auto queryResult = database_->createOffer(dto);

        OATPP_ASSERT_HTTP(queryResult->isSuccess(), Status::CODE_500, queryResult->getErrorMessage())
        OATPP_ASSERT_HTTP(queryResult->hasMoreToFetch(), Status::CODE_500, "No rows returned!")

        auto fetchResult = queryResult->fetch<oatpp::Vector<oatpp::Object<dto::OfferDto>>>();

        OATPP_ASSERT_HTTP(fetchResult->size() == 1, Status::CODE_500, "Unexpected number of rows returned!")

        return fetchResult[0];
    }

    oatpp::Object<server::dto::OfferDto>
    OfferService::getOne(
            oatpp::UInt64 const &id
    ) {
        auto queryResult = database_->getOffer(id);

        OATPP_ASSERT_HTTP(queryResult->isSuccess(), Status::CODE_500, queryResult->getErrorMessage())
        OATPP_ASSERT_HTTP(queryResult->hasMoreToFetch(), Status::CODE_404, "Offer not found")

        auto fetchResult = queryResult->fetch<oatpp::Vector<oatpp::Object<dto::OfferDto>>>();

        OATPP_ASSERT_HTTP(fetchResult->size() == 1, Status::CODE_500, "Unexpected number of rows returned!")

        return fetchResult[0];
    }

    oatpp::Object<dto::OfferPageDto>
    OfferService::search(
            oatpp::String const &query,
            oatpp::UInt64 const &limit,
            oatpp::UInt64 const &offset
    ) {
        std::string const queryTableFts = " FROM offer_fts";
        std::string const queryFilters = query->empty() ? std::string{} : " WHERE offer_fts MATCH :query";

        auto queryTotalResult = database_->executeQuery(
                "SELECT COUNT(*)" + queryTableFts + queryFilters + ";",
                {{"query", oatpp::String(query)}}
        );

        OATPP_ASSERT_HTTP(queryTotalResult->isSuccess(), Status::CODE_500, queryTotalResult->getErrorMessage())

        auto fetchTotalResult = queryTotalResult->fetch<oatpp::Vector<oatpp::Vector<oatpp::UInt64>>>();

        OATPP_ASSERT_HTTP(fetchTotalResult[0][0] > 0, Status::CODE_404, "No offers found")

        auto queryResult = database_->executeQuery(
                "SELECT offer.*" + queryTableFts +
                " INNER JOIN offer ON offer.id = offer_fts.offer_id" + queryFilters +
                " LIMIT :offset,:limit;", {
                        {"query",  oatpp::String(query)},
                        {"offset", oatpp::UInt64(offset)},
                        {"limit",  oatpp::UInt64(limit)}
                });

        auto fetchResult = queryResult->fetch<oatpp::Vector<oatpp::Object<dto::OfferDto>>>();

        auto page = dto::OfferPageDto::createShared();
        page->items = fetchResult;
        page->limit = limit;
        page->offset = offset;
        page->count = fetchTotalResult[0][0];

        return page;
    }

    oatpp::Object<dto::OfferDto>
    OfferService::putOne(
            oatpp::Object<dto::OfferDto> const &dto
    ) {
        auto queryResult = database_->replaceOffer(dto);

        OATPP_ASSERT_HTTP(queryResult->isSuccess(), Status::CODE_500, queryResult->getErrorMessage())
        OATPP_ASSERT_HTTP(queryResult->hasMoreToFetch(), Status::CODE_500, "No rows returned!")

        auto fetchResult = queryResult->fetch<oatpp::Vector<oatpp::Object<dto::OfferDto>>>();

        OATPP_ASSERT_HTTP(fetchResult->size() == 1, Status::CODE_500, "Unexpected number of rows returned!")

        return fetchResult[0];
    }

    oatpp::Object<dto::OfferDto>
    OfferService::patchOne(
            oatpp::UInt64 const &id,
            oatpp::Object<dto::OfferDto> const &dto
    ) {
        auto existing = this->getOne(id);

        existing->id = dto->id ? dto->id : existing->id;
        existing->placeId = dto->placeId ? dto->placeId : existing->placeId;
        existing->dateFrom = dto->dateFrom ? dto->dateFrom : existing->dateFrom;
        existing->dateTo = dto->dateTo ? dto->dateTo : existing->dateTo;
        existing->description = dto->description ? dto->description : existing->description;
        existing->price = dto->price ? dto->price : existing->price;

        return this->putOne(existing);
    }

    oatpp::Object<dto::StatusDto>
    OfferService::deleteOne(
            const oatpp::UInt64 &id
    ) {
        this->getOne(id); // Will throw 404 if not found

        auto queryResult = database_->deleteOffer(id);

        OATPP_ASSERT_HTTP(queryResult->isSuccess(), Status::CODE_500, queryResult->getErrorMessage())

        auto status = dto::StatusDto::createShared();
        status->status = "OK";
        status->code = Status::CODE_200.code;
        status->message = "Offer was successfully deleted";
        return status;
    }

}  // namespace server::service
