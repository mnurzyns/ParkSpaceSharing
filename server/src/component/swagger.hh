#pragma once

#include <memory>

#include <oatpp/core/macro/component.hpp>

#include <oatpp-swagger/Model.hpp>
#include <oatpp-swagger/Resources.hpp>

namespace server::component
{

class swagger
{
public:
    OATPP_CREATE_COMPONENT(::std::shared_ptr<::oatpp::swagger::DocumentInfo>, swagger_document_info)([]{
        return
            ::oatpp::swagger::DocumentInfo::Builder{}
                .setTitle("Parking space sharing")
                .setDescription("Project created by trainees of Bakcyl Programowania in 2022/2023.")
                .setVersion("0.0.1")
                .addServer("http://localhost:8000", "server on localhost")
                .build();
    }());

    OATPP_CREATE_COMPONENT(::std::shared_ptr<::oatpp::swagger::Resources>, swagger_resources)([]{
        return ::oatpp::swagger::Resources::loadResources(PSS_OATPP_SWAGGER_RES_PATH);
    }());

};

} // namespace server::component
