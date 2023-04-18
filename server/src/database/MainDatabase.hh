#pragma once

#include <oatpp/orm/DbClient.hpp>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/orm/SchemaMigration.hpp>

#include "model/UserModel.hh"

#include OATPP_CODEGEN_BEGIN(DbClient)

namespace server::database {

class MainDatabase : public oatpp::orm::DbClient
{
public:
    [[nodiscard]]
    explicit
    MainDatabase(std::shared_ptr<oatpp::orm::Executor> const& executor)
    : oatpp::orm::DbClient { executor }
    {
      oatpp::orm::SchemaMigration migration{executor};
      migration.addFile(1, PSS_DATABASE_MIGRATIONS_PATH "/001_init.sql");
      migration.migrate();

      OATPP_LOGD("database parking_space_sharing", "Migration version: %lld", executor->getSchemaVersion());
    }


    QUERY(getUser,
          "SELECT * FROM user WHERE id = :userId;",
          PARAM(oatpp::Int64, userId)
    )

    QUERY(getUserByEmail,
          "SELECT * FROM user WHERE email = :email;",
          PARAM(oatpp::String, email)
    )

    QUERY(getUserByUsername,
          "SELECT * FROM user WHERE username = :username;",
          PARAM(oatpp::String, username)
    )

    QUERY(getUserByLogin,
          "SELECT * FROM user WHERE username = :login OR email = :login;",
          PARAM(oatpp::String, login)
    )

    QUERY(searchUsers,
          "SELECT * FROM user"
          "WHERE username LIKE :query OR email LIKE :query"
          "LIMIT :limit OFFSET :offset;",
          PARAM(oatpp::String, query),
          PARAM(oatpp::UInt64, limit),
          PARAM(oatpp::UInt64, offset)
    )

  QUERY(createOneUser,
          "INSERT INTO user (username, email, password, admin)"
          "VALUES (:model.username, :model.email, :model.password, :model.admin)"
          "RETURNING *;",
          PARAM(oatpp::Object<model::UserModel>, model)
    )

};

} // namespace server::database

#include OATPP_CODEGEN_END(DbClient)
