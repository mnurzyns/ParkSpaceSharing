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
PlaceService::getPlacesByOwner(UInt64 const& id,   
                               UInt64 const& limit,
                               UInt64 const& offset)
{
    auto query_total_result = database_->countPlacesByOwner(id,limit,offset);

    OATPP_ASSERT_HTTP(query_total_result->isSuccess(),
                      Status::CODE_500,
                      query_total_result->getErrorMessage())

    auto fetch_total_result = query_total_result->fetch<Vector<Vector<UInt64>>>();

    OATPP_ASSERT_HTTP(
      fetch_total_result[0][0] > 0, Status::CODE_404, "No places found")

    auto query_result = database_->getPlacesByOwner(id,limit,offset);

    auto fetch_result = query_result->fetch<Vector<Object<PlaceDto>>>();

    auto page = PlacePageDto::createShared();
    page->items = fetch_result;
    page->limit = limit;
    page->offset = offset;
    page->count = fetch_total_result[0][0];

    return page;
}

Object<PlacePageDto>
PlaceService::search(String const& query,
                     UInt64 const& limit,
                     UInt64 const& offset)
{
    char const* query_table_fts = " FROM place_fts";
    char const* query_filters =
      query->empty() ? "" : " WHERE place_fts MATCH :query";

    auto query_total_result = database_->executeQuery(
      std::string{} + "SELECT COUNT(*)" + query_table_fts + query_filters + ";",
      { { "query", String(query) } });

    OATPP_ASSERT_HTTP(query_total_result->isSuccess(),
                      Status::CODE_500,
                      query_total_result->getErrorMessage())

    auto fetch_total_result =
      query_total_result->fetch<Vector<Vector<UInt64>>>();

    OATPP_ASSERT_HTTP(
      fetch_total_result[0][0] > 0, Status::CODE_404, "No places found")

    auto query_result = database_->executeQuery(
      std::string{} + "SELECT place.*" + query_table_fts +
        " INNER JOIN place ON place.id = place_fts.place_id" + query_filters +
        " LIMIT :offset,:limit;",
      { { "query", String(query) },
        { "offset", UInt64(offset) },
        { "limit", UInt64(limit) } });

    auto fetch_result = query_result->fetch<Vector<Object<PlaceDto>>>();

    auto page = PlacePageDto::createShared();
    page->items = fetch_result;
    page->limit = limit;
    page->offset = offset;
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
