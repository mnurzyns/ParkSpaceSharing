#pragma once

#include <memory>

#include <oatpp-sqlite/Utils.hpp>
#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/component.hpp>
#include <oatpp/web/protocol/http/Http.hpp>

#include "database/pss_db.hh"
#include "dto/page_dto.hh"
#include "dto/user_dto.hh"
#include "dto/auth_dto.hh"
#include "auth/JWT.hpp"

namespace server::service
{

    class auth_service
    {
    private:
        OATPP_COMPONENT(::std::shared_ptr<::server::database::pss_db>, database_);
        OATPP_COMPONENT(std::shared_ptr<JWT>, jwt_);

    public:
        ::oatpp::Object<::server::dto::auth_dto>
        signUp(::oatpp::Object<::server::dto::signUp_dto> const& dto);

        ::oatpp::Object<::server::dto::auth_dto>
        signIn(::oatpp::Object<::server::dto::signIn_dto> const& dto);

    };

} // namespace server::service

