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
        OATPP_ASSERT_HTTP(
          !this->getOne(dto->id), Status::CODE_409, "Offer already exists")
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
OfferService::search(Object<OfferSearchDto> const& dto)
{
    std::string const base_part = " FROM offer_fts"
                                  " INNER JOIN offer"
                                  " ON offer.id = offer_fts.offer_id";
    bool join_place = false;
    std::string const join_place_part = " INNER JOIN place"
                                        " ON place.id = offer.place_id";
    std::string filters_part{};

    if (dto->query) {
        filters_part += " offer_fts MATCH :dto.query";
    }

    if (dto->place_id) {
        if (!filters_part.empty()) {
            filters_part += " AND";
        }
        filters_part += " offer.place_id = :dto.place_id";
    }

    if (dto->date_from) {
        if (!filters_part.empty()) {
            filters_part += " AND";
        }
        filters_part += " offer.date_from >= :dto.date_from";
    }

    if (dto->date_to) {
        if (!filters_part.empty()) {
            filters_part += " AND";
        }
        filters_part += " offer.date_to <= :dto.date_to";
    }

    if (dto->price_min) {
        if (!filters_part.empty()) {
            filters_part += " AND";
        }
        filters_part += " offer.price >= :dto.price_min";
    }

    if (dto->price_max) {
        if (!filters_part.empty()) {
            filters_part += " AND";
        }
        filters_part += " offer.price <= :dto.price_max";
    }

    if (dto->owner_id) {
        join_place = true;
        if (!filters_part.empty()) {
            filters_part += " AND";
        }
        filters_part += " place.owner_id = :dto.owner_id";
    }

    if (dto->address) {
        join_place = true;
        if (!filters_part.empty()) {
            filters_part += " AND";
        }
        filters_part += " place.address LIKE :dto.address";
    }

    // TODO(papaj-na-wrotkach): filter by location

    auto query_total_result = database_->executeQuery(
      "SELECT COUNT(*)" + base_part + (join_place ? join_place_part : "") +
        (filters_part.empty() ? "" : " WHERE" + filters_part) + ";",
      { { "dto", dto } });

    OATPP_ASSERT_HTTP(query_total_result->isSuccess(),
                      Status::CODE_500,
                      query_total_result->getErrorMessage())

    auto fetch_total_result =
      query_total_result->fetch<Vector<Vector<UInt64>>>();

    OATPP_ASSERT_HTTP(
      fetch_total_result[0][0] > 0, Status::CODE_404, "No offers found")

    dto->limit = dto->limit ? dto->limit : UInt64{ 20 };
    dto->offset = dto->offset ? dto->offset : UInt64{ uint64_t{ 0 } };

    auto query_result = database_->executeQuery(
      "SELECT offer.*" + base_part + (join_place ? join_place_part : "") +
        (filters_part.empty() ? "" : " WHERE" + filters_part) +
        " LIMIT :dto.offset,:dto.limit;",
      { { "dto", dto } });

    auto fetch_result = query_result->fetch<Vector<Object<OfferDto>>>();

    auto page = OfferPageDto::createShared();
    page->items = fetch_result;
    page->limit = dto->limit;
    page->offset = dto->offset;
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
    if (dto->id && dto->id != id) {
        try {
            OATPP_ASSERT_HTTP(!getOne(dto->id),
                              Status::CODE_409,
                              "Cannot change id to id of another offer")
        } catch (HttpError& error) {
            if (error.getInfo().status != Status::CODE_404) {
                throw;
            }
        }
    }

    if (dto->date_from || dto->date_to) {
        validateDateHTTP(dto->date_from ? dto->date_from
                                        : getOne(id)->date_from,
                         dto->date_to ? dto->date_to : getOne(id)->date_to);
    }

    bool update = false;
    std::string query = "UPDATE offer SET ";
    for (auto* prop : Object<OfferDto>::getPropertiesList()) {
        if (prop->get(dto.get())) {
            if (update) {
                query += ", ";
            }
            query += std::string{} + prop->name + " = :dto." + prop->name;
            update = true;
        }
    }
    query += " WHERE id = :id RETURNING *;";

    if (update) {
        auto query_result =
          database_->executeQuery(query, { { "dto", dto }, { "id", id } });

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

    return this->getOne(id);
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
