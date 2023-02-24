#pragma once

#include <memory>

#include <oatpp/core/data/mapping/ObjectMapper.hpp>
#include <oatpp/web/protocol/http/Http.hpp>
#include <oatpp/web/protocol/http/outgoing/Response.hpp>
#include <oatpp/web/protocol/http/outgoing/ResponseFactory.hpp>
#include <oatpp/web/server/handler/ErrorHandler.hpp>

namespace server
{

class error_handler :
    public ::oatpp::web::server::handler::ErrorHandler
{
public:
    [[nodiscard]]
    explicit
    error_handler(::std::shared_ptr<::oatpp::data::mapping::ObjectMapper> object_mapper);

    ::std::shared_ptr<::oatpp::web::protocol::http::outgoing::Response>
    handleError(
        ::oatpp::web::protocol::http::Status const& status,
        ::oatpp::String const& message,
        ::oatpp::web::protocol::http::Headers const& headers
    ) override;

private:
    ::std::shared_ptr<::oatpp::data::mapping::ObjectMapper> object_mapper_;
};

} // namespace server
