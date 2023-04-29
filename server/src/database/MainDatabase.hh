#pragma once

#include <oatpp/orm/DbClient.hpp>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/orm/SchemaMigration.hpp>
#include <filesystem>
#include <set>

#include "dto/UserDto.hh"
#include "dto/PlaceDto.hh"
#include "dto/OfferDto.hh"

#include OATPP_CODEGEN_BEGIN(DbClient)

namespace server::database {

    class MainDatabase : public oatpp::orm::DbClient {
    public:
        [[nodiscard]]
        explicit
        MainDatabase(std::shared_ptr<oatpp::orm::Executor> const &executor)
                : oatpp::orm::DbClient{executor} {
            oatpp::orm::SchemaMigration migrator{executor};
            auto migrations = std::set<std::string>{};
            for (auto const &migration : std::filesystem::directory_iterator{MAIN_DATABASE_MIGRATIONS_PATH}) {
                migrations.insert(migration.path().string());
            }
            for (auto i = 0L; auto const &migration : migrations) { // NOLINT(readability-identifier-length)
                migrator.addFile(++i, migration);
                OATPP_LOGD("Database", "\tAdded migration %s", migration.c_str())
            }
            migrator.migrate();
            OATPP_LOGI("Database", "\tApplied migrations. Database version: %lld", executor->getSchemaVersion())
        }

        // User

        QUERY(createUser,
              "INSERT INTO user"
              "(id, email, username, password, role) VALUES "
              "(:user.id, :user.email, :user.username, :user.password, :user.role)"
              "RETURNING *;",
              PARAM(oatpp::Object<dto::UserDto>, user))

        QUERY(getUser,
              "SELECT * FROM user WHERE id=:id;",
              PARAM(oatpp::UInt64, id))

        QUERY(replaceUser,
              "REPLACE INTO user"
              "(id, email, username, password, role) VALUES "
              "(:user.id, :user.email, :user.username, :user.password, :user.role)"
              "RETURNING *;",
              PARAM(oatpp::Object<dto::UserDto>, user))

        QUERY(deleteUser,
              "DELETE FROM user WHERE id=:id;",
              PARAM(oatpp::UInt64, id))

        QUERY(deleteAllUsers, "DELETE FROM user;")

        // Get by UNIQUE fields

        QUERY(getUserByUsername,
              "SELECT * FROM user WHERE username=:username;",
              PARAM(oatpp::String, username))

        QUERY(getUserByEmail,
              "SELECT * FROM user WHERE email=:email;",
              PARAM(oatpp::String, email))

        // Place

        QUERY(createPlace,
              "INSERT INTO place"
              "(id, owner_id, address, latitude, longitude) VALUES "
              "(:place.id, :place.owner_id, :place.address, :place.latitude, :place.longitude)"
              "RETURNING *;",
              PARAM(oatpp::Object<dto::PlaceDto>, place))

        QUERY(getPlace,
              "SELECT * FROM place WHERE id=:id;",
              PARAM(oatpp::UInt64, id))

        QUERY(replacePlace,
              "REPLACE INTO place"
              "(id, owner_id, address, latitude, longitude) VALUES "
              "(:place.id, :place.owner_id, :place.address, :place.latitude, :place.longitude)"
              "RETURNING *;",
              PARAM(oatpp::Object<dto::PlaceDto>, place))

        QUERY(deletePlace,
              "DELETE FROM place WHERE id=:id;",
              PARAM(oatpp::UInt64, id))

        QUERY(deleteAllPlaces, "DELETE FROM place;")

        // Offer

        QUERY(createOffer,
              "INSERT INTO offer"
              "(id, place_id, date_from, date_to, description, price) VALUES "
              "(:offer.id, :offer.placeId, :offer.dateFrom, :offer.dateTo, :offer.description, :offer.price)"
              "RETURNING *;",
              PARAM(oatpp::Object<dto::OfferDto>, offer))

        QUERY(getOffer,
              "SELECT * FROM offer WHERE id=:id;",
              PARAM(oatpp::UInt64, id))

        QUERY(replaceOffer,
              "REPLACE INTO offer"
              "(id, place_id, date_from, date_to, description, price) VALUES "
              "(:offer.id, :offer.placeId, :offer.dateFrom, :offer.dateTo, :offer.description, :offer.price)"
              "RETURNING *;",
              PARAM(oatpp::Object<dto::OfferDto>, offer))

        QUERY(deleteOffer,
              "DELETE FROM offer WHERE id=:id;",
              PARAM(oatpp::UInt64, id))

        QUERY(deleteAllOffers, "DELETE FROM offer;")
    };

} // namespace server::database

#include OATPP_CODEGEN_END(DbClient)
