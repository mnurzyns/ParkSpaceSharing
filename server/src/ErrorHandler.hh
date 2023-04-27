#pragma once

#include <oatpp/web/server/handler/ErrorHandler.hpp>
#include <oatpp/core/data/mapping/ObjectMapper.hpp>
#include <oatpp/web/protocol/http/outgoing/ResponseFactory.hpp>

#include "dto/StatusDto.hh"

using ResponseFactory = oatpp::web::protocol::http::outgoing::ResponseFactory;

namespace server {

    class ErrorHandler :
            public oatpp::web::server::handler::ErrorHandler {
    public:
        [[nodiscard]]
        explicit
        ErrorHandler(std::shared_ptr<oatpp::data::mapping::ObjectMapper> objectMapper);

        std::shared_ptr<oatpp::web::protocol::http::outgoing::Response>
        handleError(
                oatpp::web::protocol::http::Status const &status,
                oatpp::String const &message,
                oatpp::web::protocol::http::Headers const &headers
        ) override;

    private:
        std::shared_ptr<oatpp::data::mapping::ObjectMapper> objectMapper_;
    };

} // namespace server
