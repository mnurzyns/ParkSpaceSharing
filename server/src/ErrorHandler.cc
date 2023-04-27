#include "ErrorHandler.hh"

namespace server {

    ErrorHandler::ErrorHandler(std::shared_ptr<oatpp::data::mapping::ObjectMapper> object_mapper) :
            objectMapper_{std::move(object_mapper)} {}

    std::shared_ptr<oatpp::web::protocol::http::outgoing::Response>
    ErrorHandler::handleError(
            oatpp::web::protocol::http::Status const &status,
            oatpp::String const &message,
            oatpp::web::protocol::http::Headers const &headers
    ) {
        auto status_dto = server::dto::StatusDto::createShared();
        status_dto->status = status.description;
        status_dto->code = status.code;
        status_dto->message = message;

        auto response = ResponseFactory::createResponse(status, status_dto, objectMapper_);
        for (auto const &pair: headers.getAll()) {
            response->putHeader(pair.first.toString(), pair.second.toString());
        }

        return response;
    }

} // namespace server
