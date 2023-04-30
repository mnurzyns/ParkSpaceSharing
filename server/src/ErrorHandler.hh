#pragma once

#include <oatpp/core/data/mapping/ObjectMapper.hpp>
#include <oatpp/web/protocol/http/outgoing/ResponseFactory.hpp>
#include <oatpp/web/server/handler/ErrorHandler.hpp>

#include "dto/StatusDto.hh"

namespace server {

using oatpp::String, oatpp::data::mapping::ObjectMapper,
  oatpp::web::protocol::http::outgoing::Response,
  oatpp::web::protocol::http::outgoing::ResponseFactory,
  oatpp::web::protocol::http::Status, server::dto::StatusDto;

class ErrorHandler : public oatpp::web::server::handler::ErrorHandler
{
  public:
    [[nodiscard]] explicit ErrorHandler(
      std::shared_ptr<ObjectMapper> object_mapper);

    std::shared_ptr<Response>
    handleError(Status const& status,
                String const& message,
                Headers const& headers) override;

  private:
    std::shared_ptr<ObjectMapper> object_mapper_;
};

} // namespace server
