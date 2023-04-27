#pragma once

#include <oatpp/core/macro/component.hpp>
#include <oatpp-swagger/Model.hpp>
#include <oatpp-swagger/Resources.hpp>

namespace server::component {

    class SwaggerComponent {

    public:
        OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::swagger::DocumentInfo>, swagger_document_info)([] {
            return
                    oatpp::swagger::DocumentInfo::Builder{}
                            .setTitle("Parking space sharing")
                            .setDescription("Project created by trainees of Bakcyl Programowania in 2022/2023.")
                            .setVersion("0.1.0")
                            .addServer("http://localhost:8000", "server on localhost")
                            .addServer("http://0.0.0.0:8000", "server on local ipv4")
                            .addServer("http://[::]:8080", "server on local ipv6")
                            .addSecurityScheme(
                                    "JWT Bearer Auth",
                                    oatpp::swagger::DocumentInfo::SecuritySchemeBuilder::DefaultBearerAuthorizationSecurityScheme()
                            ).build();
        }());

        OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::swagger::Resources>, swagger_resources)([] {
            return oatpp::swagger::Resources::loadResources(OATPP_SWAGGER_RES_PATH);
        }());

    };

} // namespace server::component
