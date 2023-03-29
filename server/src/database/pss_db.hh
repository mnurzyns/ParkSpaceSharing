#pragma once

#include <memory>

#include <oatpp/core/base/Environment.hpp>
#include <oatpp/orm/DbClient.hpp>
#include <oatpp-sqlite/Utils.hpp>
#include <oatpp/orm/Executor.hpp>
#include <oatpp/orm/SchemaMigration.hpp>
#include <oatpp-sqlite/orm.hpp>

#include "dto/user_dto.hh"
#include "dto/signIn_dto.hh"
#include "dto/signUp_dto.hh"
#include "dto/auth_dto.hh"
#include "dto/offer_dto.hh"

#include OATPP_CODEGEN_BEGIN(DbClient)

namespace server::database
{

class pss_db :
    public ::oatpp::orm::DbClient
{
public:
    [[nodiscard]]
    explicit
    pss_db(::std::shared_ptr<::oatpp::orm::Executor> const& executor)
        : ::oatpp::orm::DbClient{executor}
    {
        ::oatpp::orm::SchemaMigration migration{executor};
        migration.addFile(1, PSS_DATABASE_MIGRATIONS_PATH "/001_init.sql");
        migration.migrate();

        OATPP_LOGD("database parking_space_sharing", "Migration version: %lld", executor->getSchemaVersion());
    }

    QUERY(
        signIn,
        "SELECT id FROM user WHERE email = :email AND password = :password;",
        PARAM(::oatpp::String, email),
        PARAM(::oatpp::String, password)
    )

    QUERY(
        is_exist,
        "SELECT id FROM user WHERE email = :email OR username = :username;",
        PARAM(::oatpp::String, email),
        PARAM(::oatpp::String, username)
    )

    QUERY(
        is_offer_exist,
        "SELECT id FROM offer WHERE parking_space_id = :id_parking_space;",
        PARAM(oatpp::UInt32, id_parking_space)
    )

    QUERY(
        signUp,
        "INSERT INTO user(username, email, password) VALUES(:form.username, :form.email, :form.password);",
        PARAM(::oatpp::Object<::server::dto::signUp_dto>, form)
    )

    QUERY(
        get_users,
        "SELECT * FROM user;"
    )

    QUERY(
        get_user_byId,
        "SELECT * FROM user WHERE id=:id;",
        PARAM(oatpp::UInt32, id)
    )

    QUERY(
        create_user,
        "INSERT INTO user(username, email, password, token, admin) VALUES(:user.username, :user.email, :user.password, :user.token, :user.admin);",
        PARAM(::oatpp::Object<::server::dto::user_dto>, user)
    )

    QUERY(
        get_offers,
        "SELECT * FROM offer;"
    )

    QUERY(
        get_offer_byId,
        "SELECT * FROM offer WHERE id = :id;",
        PARAM(oatpp::UInt32, id)
    )    

    QUERY(
        create_offer,
        "INSERT INTO offer(parking_space_id, description) VALUES(:offer.id_parkingSpace, :offer.description);",
        PARAM(::oatpp::Object<::server::dto::offer_dto>, offer)
    )

    QUERY(
        delete_user_byId,
        "DELETE FROM user WHERE id = :id;",
        PARAM(oatpp::UInt32, id)
    )
};

} // namespace server::database

#include OATPP_CODEGEN_END(DbClient)
