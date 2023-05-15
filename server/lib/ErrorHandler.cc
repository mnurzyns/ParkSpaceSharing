#include "ErrorHandler.hh"

namespace server {

ErrorHandler::ErrorHandler(std::shared_ptr<ObjectMapper> object_mapper)
  : object_mapper_{ std::move(object_mapper) }
{
}

std::shared_ptr<Response>
ErrorHandler::handleError(Status const& status,
                          String const& message,
                          Headers const& headers)
{
    auto status_dto = StatusDto::createShared();
    status_dto->status = status.description;
    status_dto->code = status.code;
    status_dto->message = message;

    auto response =
      ResponseFactory::createResponse(status, status_dto, object_mapper_);
    for (auto const& pair : headers.getAll()) {
        response->putHeader(pair.first.toString(), pair.second.toString());
    }

    return response;
}

} // namespace server
