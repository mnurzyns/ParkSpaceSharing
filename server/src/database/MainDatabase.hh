#pragma once

#include <oatpp/orm/DbClient.hpp>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/orm/SchemaMigration.hpp>

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
            oatpp::orm::SchemaMigration migration{executor};
            migration.addFile(1, PSS_DATABASE_MIGRATIONS_PATH "/001_init.sql");
            migration.migrate();

            OATPP_LOGD("database parking_space_sharing", "Migration version: %lld", executor->getSchemaVersion());
        }

        // User

        QUERY(createUser,
              "INSERT INTO user"
              "(username, email, password, role) VALUES "
              "(:user.username, :user.email, :user.password, :user.role)"
              "RETURNING id;",
              PARAM(oatpp::Object<dto::UserDto>, user))

        QUERY(getUser,
              "SELECT * FROM user WHERE id=:userId;",
              PARAM(oatpp::UInt64, userId))

        QUERY(replaceUser,
              "REPLACE INTO user"
              "(id, username, email, password, role) VALUES "
              "(:user.id, :user.username, :user.email, :user.password, :user.role)"
              "RETURNING *;",
              PARAM(oatpp::Object<dto::UserDto>, user))

        QUERY(deleteUser,
              "DELETE FROM user WHERE id=:userId;",
              PARAM(oatpp::UInt64, userId))

        QUERY(deleteAllUsers, "DELETE FROM user;")

        // For AuthService

        QUERY(getUserByUsername,
              "SELECT * FROM user WHERE username=:username;",
              PARAM(oatpp::String, username))

        QUERY(getUserByEmail,
              "SELECT * FROM user WHERE email=:email;",
              PARAM(oatpp::String, email))

        // Place

        QUERY(createPlace,
              "INSERT INTO place"
              "(owner_id, address, latitude, longitude) VALUES "
              "(:place.owner_id, :place.address, :place.latitude, :place.longitude)"
              "RETURNING *;",
              PARAM(oatpp::Object<dto::PlaceDto>, place))

        QUERY(getPlace,
              "SELECT * FROM place WHERE id=:placeId;",
              PARAM(oatpp::UInt64, placeId))

        QUERY(replacePlace,
              "REPLACE INTO place"
              "(id, owner_id, address, latitude, longitude) VALUES "
              "(:place.id, :place.owner_id, :place.address, :place.latitude, :place.longitude)"
              "RETURNING *;",
              PARAM(oatpp::Object<dto::PlaceDto>, place))

        QUERY(deletePlace,
              "DELETE FROM place WHERE id=:placeId;",
              PARAM(oatpp::UInt64, placeId))

        QUERY(deleteAllPlaces, "DELETE FROM place;")

        // Offer

        QUERY(createOffer,
              "INSERT INTO offer"
              "(place_id, description, price, start_date, end_date) VALUES "
              "(:offer.placeId, :offer.description, :offer.price, :offer.startDate, :offer.endDate)"
              "RETURNING *;",
              PARAM(oatpp::Object<dto::OfferDto>, offer))

        QUERY(getOffer,
              "SELECT * FROM offer WHERE id=:offerId;",
              PARAM(oatpp::UInt64, offerId))

        QUERY(replaceOffer,
              "REPLACE INTO offer"
              "(id, place_id, description, price, start_date, end_date) VALUES "
              "(:offer.id, :offer.placeId, :offer.description, :offer.price, :offer.startDate, :offer.endDate)"
              "RETURNING *;",
              PARAM(oatpp::Object<dto::OfferDto>, offer))

        QUERY(deleteOffer,
              "DELETE FROM offer WHERE id=:offerId;",
              PARAM(oatpp::UInt64, offerId))

        QUERY(deleteAllOffers, "DELETE FROM offer;")
    };

} // namespace server::database

#include OATPP_CODEGEN_END(DbClient)
