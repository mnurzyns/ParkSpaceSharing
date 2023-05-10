#pragma once

#include <filesystem>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/orm/DbClient.hpp>
#include <oatpp/orm/SchemaMigration.hpp>

#include "dto/OfferDto.hh"
#include "dto/PlaceDto.hh"
#include "dto/UserDto.hh"

#include OATPP_CODEGEN_BEGIN(DbClient)

namespace server::database {

using namespace oatpp::data::mapping::type; // NOLINT
using namespace server::dto;                // NOLINT
using oatpp::Object;

class MainDatabase : public oatpp::orm::DbClient
{
  public:
    [[nodiscard]] explicit MainDatabase(
      std::shared_ptr<oatpp::orm::Executor> const& executor);

    // User

    QUERY(createUser,
          "INSERT INTO user"
          "(id, email, phone, username, password, role) VALUES "
          "(:user.id, :user.email, :user.phone, :user.username, "
          ":user.password, :user.role)"
          "RETURNING *;",
          PARAM(Object<UserDto>, user))

    QUERY(getUser, "SELECT * FROM user WHERE id=:id;", PARAM(UInt64, id))

    QUERY(replaceUser,
          "REPLACE INTO user"
          "(id, email, phone, username, password, role) VALUES "
          "(:user.id, :user.email, :user.phone, :user.username, "
          ":user.password, :user.role)"
          "RETURNING *;",
          PARAM(Object<UserDto>, user))

    QUERY(deleteUser, "DELETE FROM user WHERE id=:id;", PARAM(UInt64, id))

    QUERY(deleteAllUsers, "DELETE FROM user;")

    // Get by UNIQUE fields

    QUERY(getUserByUsername,
          "SELECT * FROM user WHERE username=:username;",
          PARAM(String, username))

    QUERY(getUserByEmail,
          "SELECT * FROM user WHERE email=:email;",
          PARAM(String, email))

    QUERY(getUserByPhone,
          "SELECT * FROM user WHERE phone=:phone;",
          PARAM(String, phone))

    // Place

    QUERY(createPlace,
          "INSERT INTO place"
          "(id, owner_id, address, latitude, longitude) VALUES "
          "(:place.id, :place.owner_id, :place.address, :place.latitude, "
          ":place.longitude)"
          "RETURNING *;",
          PARAM(Object<PlaceDto>, place))

    QUERY(getPlace, "SELECT * FROM place WHERE id=:id;", PARAM(UInt64, id))

    QUERY(getPlacesByOwner, "SELECT * FROM place WHERE owner_id=:id LIMIT :offset, :limit;", 
          PARAM(UInt64, id),
          PARAM(UInt64, limit),
          PARAM(UInt64, offset))

    QUERY(countPlacesByOwner, "SELECT COUNT(*) FROM place WHERE owner_id=:id;", 
          PARAM(UInt64, id))

    QUERY(replacePlace,
          "REPLACE INTO place"
          "(id, owner_id, address, latitude, longitude) VALUES "
          "(:place.id, :place.owner_id, :place.address, :place.latitude, "
          ":place.longitude)"
          "RETURNING *;",
          PARAM(Object<PlaceDto>, place))

    QUERY(deletePlace, "DELETE FROM place WHERE id=:id;", PARAM(UInt64, id))

    QUERY(deleteAllPlaces, "DELETE FROM place;")

    // Offer

    QUERY(createOffer,
          "INSERT INTO offer"
          "(id, place_id, date_from, date_to, description, price) VALUES "
          "(:offer.id, :offer.place_id, :offer.date_from, :offer.date_to, "
          ":offer.description, :offer.price)"
          "RETURNING *;",
          PARAM(Object<OfferDto>, offer))

    QUERY(getOffer, "SELECT * FROM offer WHERE id=:id;", PARAM(UInt64, id))

    QUERY(countOffersByOwner, "SELECT COUNT(offer.id) FROM offer INNER JOIN place ON place.id = offer.place_id WHERE place.owner_id = :id ;", 
          PARAM(UInt64, id))

    QUERY(getOffersByOwner, "SELECT offer.* FROM offer INNER JOIN place ON place.id = offer.place_id WHERE place.owner_id = :id LIMIT :offset, :limit;", 
          PARAM(UInt64, id),
          PARAM(UInt64, limit),
          PARAM(UInt64, offset))

    QUERY(replaceOffer,
          "REPLACE INTO offer"
          "(id, place_id, date_from, date_to, description, price) VALUES "
          "(:offer.id, :offer.place_id, :offer.date_from, :offer.date_to, "
          ":offer.description, :offer.price)"
          "RETURNING *;",
          PARAM(Object<OfferDto>, offer))

    QUERY(deleteOffer, "DELETE FROM offer WHERE id=:id;", PARAM(UInt64, id))

    QUERY(deleteAllOffers, "DELETE FROM offer;")
};

} // namespace server::database

#include OATPP_CODEGEN_END(DbClient)
