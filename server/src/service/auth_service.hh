#pragma once

#include <memory>

#include "database/pss_db.hh"
#include "dto/page_dto.hh"
#include "dto/user_dto.hh"
#include "dto/auth_dto.hh"
#include "dto/status_dto.hh"
#include "auth/JWT.hpp"

#include <oatpp/web/protocol/http/Http.hpp>
#include <oatpp/core/macro/component.hpp>
#include <oatpp/core/Types.hpp>
#include <string>


namespace server::service
{

    class auth_service
    {
    private:
        OATPP_COMPONENT(::std::shared_ptr<::server::database::pss_db>, database_);  // Inject database component
        OATPP_COMPONENT(std::shared_ptr<JWT>, jwt_);                                // Inject JWT component

    public:
        ::oatpp::Object<::server::dto::auth_dto>
        signUp(::oatpp::Object<::server::dto::signUp_dto> const& dto);

        ::oatpp::Object<::server::dto::auth_dto>
        signIn(::oatpp::Object<::server::dto::signIn_dto> const& dto);

    };

} // namespace server::service

