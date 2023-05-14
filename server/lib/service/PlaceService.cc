#include "PlaceService.hh"

namespace server::service {

std::shared_ptr<PlaceService>
PlaceService::createShared()
{
    return std::make_shared<PlaceService>();
}

Object<PlaceDto>
PlaceService::createOne(Object<PlaceDto> const& dto)
{
    try {
        OATPP_ASSERT_HTTP(
          !this->getOne(dto->id), Status::CODE_409, "Place already exists")
    } catch (HttpError& e) {
        if (e.getInfo().status != Status::CODE_404) {
            throw;
        }
    }

    auto query_result = database_->createPlace(dto);

    OATPP_ASSERT_HTTP(query_result->isSuccess(),
                      Status::CODE_500,
                      query_result->getErrorMessage())

    OATPP_ASSERT_HTTP(
      query_result->hasMoreToFetch(), Status::CODE_500, "No rows returned!")

    auto fetch_result = query_result->fetch<Vector<Object<PlaceDto>>>();

    OATPP_ASSERT_HTTP(fetch_result->size() == 1,
                      Status::CODE_500,
                      "Unexpected number of rows returned!")

    return fetch_result[0];
}

Object<PlaceDto>
PlaceService::getOne(UInt64 const& id)
{
    auto query_result = database_->getPlace(id);

    OATPP_ASSERT_HTTP(query_result->isSuccess(),
                      Status::CODE_500,
                      query_result->getErrorMessage())

    OATPP_ASSERT_HTTP(
      query_result->hasMoreToFetch(), Status::CODE_404, "Place not found")

    auto fetch_result = query_result->fetch<Vector<Object<PlaceDto>>>();

    OATPP_ASSERT_HTTP(fetch_result->size() == 1,
                      Status::CODE_500,
                      "Unexpected number of rows returned!")

    return fetch_result[0];
}

Object<PlacePageDto>
PlaceService::search(Object<PlaceSearchDto> const& dto)
{
    std::string const base_part = " FROM place";
    std::string filters_part{};

    if (dto->query) {
        filters_part += " place.id IN (SELECT place_fts.ROWID"
                        " FROM place_fts"

                        " INNER JOIN user_fts"
                        " ON user_fts.ROWID = place_fts.owner_id"

                        " WHERE place_fts.ROWID IN (SELECT ROWID"
                        " FROM place_fts WHERE place_fts MATCH :dto.query)"

                        " OR user_fts.ROWID IN (SELECT ROWID"
                        " FROM user_fts WHERE user_fts MATCH :dto.query))";
    }

    if (dto->owner_id) {
        if (!filters_part.empty()) {
            filters_part += " AND";
        }
        filters_part += " place.owner_id = :dto.owner_id";
    }

    // TODO(papaj-na-wrotkach): filter by location

    auto query_total_result = database_->executeQuery(
      "SELECT COUNT(*)" + base_part +
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
      "SELECT place.*" + base_part +
        (filters_part.empty() ? "" : " WHERE" + filters_part) +
        " LIMIT :dto.offset,:dto.limit;",
      { { "dto", dto } });

    auto fetch_result = query_result->fetch<Vector<Object<PlaceDto>>>();

    auto page = PlacePageDto::createShared();
    page->items = fetch_result;
    page->limit = dto->limit;
    page->offset = dto->offset;
    page->count = fetch_total_result[0][0];

    return page;
}

Object<PlaceDto>
PlaceService::putOne(Object<PlaceDto> const& dto)
{
    auto query_result = database_->replacePlace(dto);

    OATPP_ASSERT_HTTP(query_result->isSuccess(),
                      Status::CODE_500,
                      query_result->getErrorMessage())

    OATPP_ASSERT_HTTP(
      query_result->hasMoreToFetch(), Status::CODE_500, "No rows returned!")

    auto fetch_result = query_result->fetch<Vector<Object<PlaceDto>>>();

    OATPP_ASSERT_HTTP(fetch_result->size() == 1,
                      Status::CODE_500,
                      "Unexpected number of rows returned!")

    return fetch_result[0];
}

Object<PlaceDto>
PlaceService::patchOne(UInt64 const& id, Object<PlaceDto> const& dto)
{
    if (dto->id && dto->id != id) {
        try {
            OATPP_ASSERT_HTTP(!getOne(dto->id),
                              Status::CODE_409,
                              "Cannot change id to id of another place")
        } catch (HttpError& error) {
            if (error.getInfo().status != Status::CODE_404) {
                throw;
            }
        }
    }

    bool update = false;
    std::string query = "UPDATE place SET ";
    for (auto* prop : Object<PlaceDto>::getPropertiesList()) {
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

        auto fetch_result = query_result->fetch<Vector<Object<PlaceDto>>>();

        OATPP_ASSERT_HTTP(fetch_result->size() == 1,
                          Status::CODE_500,
                          "Unexpected number of rows returned!")

        return fetch_result[0];
    }

    return this->getOne(id);
}

Object<StatusDto>
PlaceService::deleteOne(UInt64 const& id)
{
    this->getOne(id); // Will throw 404 if not found

    auto query_result = database_->deletePlace(id);

    OATPP_ASSERT_HTTP(query_result->isSuccess(),
                      Status::CODE_500,
                      query_result->getErrorMessage())

    auto status = StatusDto::createShared();
    status->status = "OK";
    status->code = Status::CODE_200.code;
    status->message = "Place deleted successfully";

    return status;
}

} // namespace server::service
