#pragma once

#include <memory>

#include <oatpp/core/base/Environment.hpp>
#include <oatpp/orm/DbClient.hpp>
#include <oatpp/orm/Executor.hpp>
#include <oatpp/orm/SchemaMigration.hpp>

#include <oatpp-sqlite/orm.hpp>

#include "dto/user.hh"

#include OATPP_CODEGEN_BEGIN(DbClient)

namespace server::database
{

class parking_space_sharing :
    public ::oatpp::orm::DbClient
{
public:
    [[nodiscard]]
    explicit
    parking_space_sharing(::std::shared_ptr<::oatpp::orm::Executor> const& executor)
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
        create_user,
        "INSERT INTO user(username, password) VALUES(:user.username, :user.password);",
        PARAM(::oatpp::Object<::server::dto::user>, user)
    )

    QUERY(
        get_parking_spaces,
        "SELECT * FROM parking_space;"
    )
};

} // namespace server::database

#include OATPP_CODEGEN_END(DbClient)
