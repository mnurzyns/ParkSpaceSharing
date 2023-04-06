#pragma once

#include <memory>

#include <oatpp/core/base/Environment.hpp>
#include <oatpp/orm/DbClient.hpp>
#include <oatpp-sqlite/Utils.hpp>
#include <oatpp/orm/Executor.hpp>
#include <oatpp/orm/SchemaMigration.hpp>
#include <oatpp-sqlite/orm.hpp>

#include "dto/parkingSpace_dto.hh"
#include "dto/user_dto.hh"
#include "dto/signIn_dto.hh"
#include "dto/signUp_dto.hh"
#include "dto/auth_dto.hh"
#include "dto/offer_dto.hh"

#include OATPP_CODEGEN_BEGIN(DbClient)

namespace server { namespace database
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
        "SELECT * FROM user WHERE email = :email AND password = :password;",
        PARAM(::oatpp::String, email),
        PARAM(::oatpp::String, password)
    )
    QUERY(
        signUp,
        "INSERT INTO user(username, email, password) VALUES(:form.username, :form.email, :form.password);",
        PARAM(::oatpp::Object<::server::dto::signUp_dto>, form)
    )
    QUERY(
        is_exist,
        "SELECT id FROM user WHERE email = :email OR username = :username;",
        PARAM(::oatpp::String, email),
        PARAM(::oatpp::String, username)
    )


    QUERY(
        get_users,
        "SELECT * FROM user;"
    )
    QUERY(
        get_user_byId,
        "SELECT * FROM user WHERE id=:us_id;",
        PARAM(oatpp::UInt32, us_id)
    )
    QUERY(
        create_user,
        "INSERT INTO user(username, email, password, token, admin) VALUES(:user.username, :user.email, :user.password, :user.token, :user.admin);",
        PARAM(::oatpp::Object<::server::dto::user_dto>, user)
    )
    QUERY(
        delete_user,
        "DELETE FROM user WHERE id = :us_id;",
        PARAM(oatpp::UInt32, us_id)
    )

    QUERY(
        get_offers,
        "SELECT * FROM offer;"
    )
    QUERY(
        get_myOffers,
        "SELECT offer.* FROM offer INNER JOIN parking_space ON offer.parking_space_id = parking_space.id AND parking_space.owner_id = :us_id;",
        PARAM(oatpp::UInt32, us_id)
    )
    QUERY(
        create_offer,
        "INSERT INTO offer(parking_space_id, description) VALUES(:offer.id_parkingSpace, :offer.description);",
        PARAM(::oatpp::Object<::server::dto::offer_dto>, offer)
    )
    QUERY(
        is_offer_exist,
        "SELECT id FROM offer WHERE parking_space_id = :id_parking_space;",
        PARAM(oatpp::UInt32, id_parking_space)
    )
    QUERY(
        get_offer_byId,
        "SELECT * FROM offer WHERE id = :of_id;",
        PARAM(oatpp::UInt32, of_id)
    )
    QUERY(
        delete_myOffer,
        "DELETE * FROM (SELECT offer.* FROM offer INNER JOIN parking_space ON offer.parking_space_id = parking_space.id AND parking_space.owner_id = :us_id) WHERE id = :of_id;",
        PARAM(oatpp::UInt32, of_id),
        PARAM(oatpp::UInt32, us_id)
    )
    QUERY(
        delete_offer,
        "DELETE FROM offer WHERE id = :of_id;",
        PARAM(oatpp::UInt32, of_id)
    )


    QUERY(
        get_parkingSpace,
        "SELECT * FROM parking_space;"
    )
    QUERY(
        get_myparkingSpace,
        "SELECT * FROM parking_space WHERE user_id = :ps_id;",
        PARAM(oatpp::UInt32, ps_id)
    )
    QUERY(
        create_parkingSpace,
        "INSERT INTO parkingSpace(owner_id, location) VALUES(:parkingSpace.owner_id, :parkingSpace.location);",
        PARAM(::oatpp::Object<::server::dto::parkingSpace_dto>, parkingSpace)
    )
    QUERY(
        get_parkingSpace_byId,
        "SELECT * FROM parkingSpace WHERE id = :ps_id;",
        PARAM(oatpp::UInt32, ps_id)
    )
    QUERY(
        delete_parkingSpace,
        "DELETE FROM parkingSpace WHERE id = :ps_id;",
        PARAM(oatpp::UInt32, ps_id)
    ) 

};

} } // namespace server::database

#include OATPP_CODEGEN_END(DbClient)
