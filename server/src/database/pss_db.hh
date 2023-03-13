#pragma once

#include <memory>

#include <oatpp/core/base/Environment.hpp>
#include <oatpp/orm/DbClient.hpp>
#include <oatpp/orm/Executor.hpp>
#include <oatpp/orm/SchemaMigration.hpp>

#include <oatpp-sqlite/orm.hpp>

#include "dto/user_dto.hh"
#include "dto/signIn_dto.hh"
#include "dto/signUp_dto.hh"
#include "dto/auth_dto.hh"

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
        get_users,
        "SELECT * FROM user;"
    )

    QUERY(
        get_user,
        "SELECT * FROM user WHERE id=:id;",
        PARAM(::oatpp::UInt32, id)
    )

    QUERY(
        auth_user,
        "SELECT * FROM user WHERE token = :token;",
        PARAM(::oatpp::String, token)
    )

    QUERY(
        signIn_auth,
        "SELECT token FROM user WHERE username = :form.username ,password = :form.password;",
        PARAM(::oatpp::Object<::server::dto::signIn_dto>, form)
    )

    QUERY(
        signUp,
        "INSERT INTO user(username, password, token) VALUES(:form.username, :form.password, :token);",
        PARAM(::oatpp::Object<::server::dto::signUp_dto>, form),
        PARAM(::oatpp::String, token)
    )

    QUERY(
        create_user,
        "INSERT INTO user(username, password, token, admin) VALUES(:user.username, :user.password, :user.token, :user.admin);",
        PARAM(::oatpp::Object<::server::dto::user_dto>, user)
    )

    QUERY(
        get_offers,
        "SELECT * FROM offer;"
    )

    //na potrzeby testowania
    QUERY(
        delete_users,
        "DELETE FROM user;"
    )
};

} // namespace server::database

#include OATPP_CODEGEN_END(DbClient)
