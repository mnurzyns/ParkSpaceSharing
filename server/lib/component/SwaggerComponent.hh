#pragma once

#include <filesystem>
#include <oatpp-swagger/Model.hpp>
#include <oatpp-swagger/Resources.hpp>
#include <oatpp/core/macro/component.hpp>

#include "Config.hh"

namespace server::component {

class SwaggerComponent
{

  public:
    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::swagger::DocumentInfo>,
                           swagger_document_info)
    ([] {
        return oatpp::swagger::DocumentInfo::Builder{}
          .setTitle("Parking space sharing")
          .setDescription("Project created by trainees of"
                          "Bakcyl Programowania in 2022/2023.")
          .setVersion("0.1.0")
          .addServer("http://localhost:8000", "local server (localhost)")
          .addServer("http://0.0.0.0:8000", "local server (IPv4)")
          .addServer("http://[::]:8080", "local server (IPv6)")
          .addSecurityScheme(
            "JWT Bearer Auth",
            oatpp::swagger::DocumentInfo::SecuritySchemeBuilder::
              DefaultBearerAuthorizationSecurityScheme())
          .build();
    }());

    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::swagger::Resources>,
                           swagger_resources)
    ([] {
        std::string_view res_path =
          server::Config::getInstance().swagger_res_path;
        if (!std::filesystem::exists(res_path)) {
            res_path = OATPP_SWAGGER_RES_PATH;
            if (!std::filesystem::exists(res_path)) {
                throw std::runtime_error{
                    "server.swagger.res_path does not point to a valid "
                    "directory. Check your configuration."
                };
            }
        }

        OATPP_LOGI("Swagger",
                   "\tUsing \"%*s\" as swagger resource path.",
                   res_path.length(),
                   res_path.data())

        return oatpp::swagger::Resources::loadResources(
          String{ std::string{ res_path } });
    }());
};

} // namespace server::component
