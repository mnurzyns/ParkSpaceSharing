#include "OfferService.hh"

namespace server::service {

std::shared_ptr<OfferService>
OfferService::createShared()
{
    return std::make_shared<OfferService>();
}

void
validateDateHTTP(UInt64 const& date_from, UInt64 const& date_to)
{
    OATPP_ASSERT_HTTP(
      date_from <= date_to, Status::CODE_400, "Invalid time range");
}

Object<OfferDto>
OfferService::createOne(Object<OfferDto> const& dto)
{
    validateDateHTTP(dto->date_from, dto->date_to);

    try {
        this->getOne(dto->id); // Will throw 404 if not found
        OATPP_ASSERT_HTTP(false, Status::CODE_409, "Offer already exists")
    } catch (oatpp::web::protocol::http::HttpError& e) {
        if (e.getInfo().status != Status::CODE_404) {
            throw;
        }
    }

    auto query_result = database_->createOffer(dto);

    OATPP_ASSERT_HTTP(query_result->isSuccess(),
                      Status::CODE_500,
                      query_result->getErrorMessage())

    OATPP_ASSERT_HTTP(
      query_result->hasMoreToFetch(), Status::CODE_500, "No rows returned!")

    auto fetch_result = query_result->fetch<Vector<Object<OfferDto>>>();

    OATPP_ASSERT_HTTP(fetch_result->size() == 1,
                      Status::CODE_500,
                      "Unexpected number of rows returned!")

    return fetch_result[0];
}

Object<OfferDto>
OfferService::getOne(UInt64 const& id)
{
    auto query_result = database_->getOffer(id);

    OATPP_ASSERT_HTTP(query_result->isSuccess(),
                      Status::CODE_500,
                      query_result->getErrorMessage())

    OATPP_ASSERT_HTTP(
      query_result->hasMoreToFetch(), Status::CODE_404, "Offer not found")

    auto fetch_result = query_result->fetch<Vector<Object<OfferDto>>>();

    OATPP_ASSERT_HTTP(fetch_result->size() == 1,
                      Status::CODE_500,
                      "Unexpected number of rows returned!")

    return fetch_result[0];
}

Object<OfferPageDto>
OfferService::search(String const& query,
                     UInt64 const& limit,
                     UInt64 const& offset)
{
    char const* query_table_fts = " FROM offer_fts";
    char const* query_filters =
      query->empty() ? "" : " WHERE offer_fts MATCH :query";

    auto query_total_result = database_->executeQuery(
      std::string{} + "SELECT COUNT(*)" + query_table_fts + query_filters + ";",
      { { "query", String(query) } });

    OATPP_ASSERT_HTTP(query_total_result->isSuccess(),
                      Status::CODE_500,
                      query_total_result->getErrorMessage())

    auto fetch_total_result =
      query_total_result->fetch<Vector<Vector<UInt64>>>();

    OATPP_ASSERT_HTTP(
      fetch_total_result[0][0] > 0, Status::CODE_404, "No offers found")

    auto query_result = database_->executeQuery(
      std::string{} + "SELECT offer.*" + query_table_fts +
        " INNER JOIN offer ON offer.id = offer_fts.offer_id" + query_filters +
        " LIMIT :offset,:limit;",
      { { "query", String(query) },
        { "offset", UInt64(offset) },
        { "limit", UInt64(limit) } });

    auto fetch_result = query_result->fetch<Vector<Object<OfferDto>>>();

    auto page = OfferPageDto::createShared();
    page->items = fetch_result;
    page->limit = limit;
    page->offset = offset;
    page->count = fetch_total_result[0][0];

    return page;
}

Object<OfferDto>
OfferService::putOne(Object<OfferDto> const& dto)
{
    validateDateHTTP(dto->date_from, dto->date_to);

    auto query_result = database_->replaceOffer(dto);

    OATPP_ASSERT_HTTP(query_result->isSuccess(),
                      Status::CODE_500,
                      query_result->getErrorMessage())

    OATPP_ASSERT_HTTP(
      query_result->hasMoreToFetch(), Status::CODE_500, "No rows returned!")

    auto fetch_result = query_result->fetch<Vector<Object<OfferDto>>>();

    OATPP_ASSERT_HTTP(fetch_result->size() == 1,
                      Status::CODE_500,
                      "Unexpected number of rows returned!")

    return fetch_result[0];
}

Object<OfferDto>
OfferService::patchOne(UInt64 const& id, Object<OfferDto> const& dto)
{
    auto existing = this->getOne(id);

    existing->id = dto->id ? dto->id : existing->id;
    existing->place_id = dto->place_id ? dto->place_id : existing->place_id;
    existing->date_from = dto->date_from ? dto->date_from : existing->date_from;
    existing->date_to = dto->date_to ? dto->date_to : existing->date_to;
    existing->description =
      dto->description ? dto->description : existing->description;
    existing->price = dto->price ? dto->price : existing->price;

    validateDateHTTP(existing->date_from, existing->date_to);

    return this->putOne(existing);
}

Object<StatusDto>
OfferService::deleteOne(UInt64 const& id)
{
    this->getOne(id); // Will throw 404 if not found

    auto query_result = database_->deleteOffer(id);

    OATPP_ASSERT_HTTP(query_result->isSuccess(),
                      Status::CODE_500,
                      query_result->getErrorMessage())

    auto status = StatusDto::createShared();
    status->status = "OK";
    status->code = Status::CODE_200.code;
    status->message = "Offer deleted successfully";

    return status;
}

} // namespace server::service
