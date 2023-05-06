#pragma once

#include "Context.hh"

namespace tests {

/*
  Requires working PlaceController POST endpoint.
 */
void
offerPostTest(TestEnvironment const& env, AuthContext const& auth);

/*
  Requires working POST endpoint.
 */
oatpp::Object<server::dto::OfferDto>
createDummyOffer(TestEnvironment const& env, AuthContext const& auth);

void
offerGetByIdTest(TestEnvironment const& env, AuthContext const& auth);

void
offerPatchTest(TestEnvironment const& env, AuthContext const& auth);

void
offerPutTest(TestEnvironment const& env, AuthContext const& auth);

/*
  Requires working PlaceController DELETE endpoint.
 */
void
offerDeleteTest(TestEnvironment const& env, AuthContext const& auth);

} // namespace tests
