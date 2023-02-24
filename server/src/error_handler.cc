#include "error_handler.hh"

#include <memory>

#include <oatpp/core/data/mapping/ObjectMapper.hpp>
#include <oatpp/web/protocol/http/outgoing/Response.hpp>
#include <oatpp/web/protocol/http/outgoing/ResponseFactory.hpp>

#include "dto/status.hh"

namespace server
{

error_handler::error_handler(::std::shared_ptr<::oatpp::data::mapping::ObjectMapper> object_mapper) :
    object_mapper_{::std::move(object_mapper)} {}

::std::shared_ptr<::oatpp::web::protocol::http::outgoing::Response>
error_handler::handleError(::oatpp::web::protocol::http::Status const& status, ::oatpp::String const& message, ::oatpp::web::protocol::http::Headers const& headers)
{
    auto status_dto = ::server::dto::status::createShared();
    status_dto->status = "ERROR";
    status_dto->code = status.code;
    status_dto->message = message;

    auto response = ::oatpp::web::protocol::http::outgoing::ResponseFactory::createResponse(status, status_dto, object_mapper_);
    for(auto const& pair : headers.getAll()) {
        response->putHeader(pair.first.toString(), pair.second.toString());
    }

    return response;
}

} // namespace server
