#pragma once

#include "Context.hh"

namespace tests {

void
placePostTest(TestEnvironment const& env, AuthContext const& auth);

void
placeGetByIdTest(TestEnvironment const& env, AuthContext const& auth);

void
placePatchTest(TestEnvironment const& env, AuthContext const& auth);

void
placePutTest(TestEnvironment const& env, AuthContext const& auth);

void
placeDeleteTest(TestEnvironment const& env, AuthContext const& auth);

} // namespace tests
