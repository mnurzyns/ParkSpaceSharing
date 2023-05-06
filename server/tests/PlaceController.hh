#pragma once

#include "Context.hh"

namespace tests {

void
placePostTest(TestEnvironment const& env, AuthContext const& auth);

/*
  Requires working POST endpoint.
 */
oatpp::Object<server::dto::PlaceDto>
createDummyPlace(TestEnvironment const& env, AuthContext const& auth);

void
placeGetByIdTest(TestEnvironment const& env, AuthContext const& auth);

void
placePatchTest(TestEnvironment const& env, AuthContext const& auth);

void
placePutTest(TestEnvironment const& env, AuthContext const& auth);

/*
  Requires working GET endpoint.
 */
void
placeDeleteTest(TestEnvironment const& env, AuthContext const& auth);

} // namespace tests
