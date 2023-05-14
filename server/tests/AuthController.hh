#pragma once

#include "Context.hh"

namespace tests {

void
signupPostTest(TestEnvironment const& env);

/*
  Requires working /signup POST endpoint.
 */
void
signinPostTest(TestEnvironment const& env);

/*
  Requires working signup endpoint.
 */
oatpp::Object<server::dto::SignUpDto>
signupDummyUser(TestEnvironment const& env);

/*
  Requires working signup and signin endpoint.
 */
AuthContext
signinAsDummyUser(TestEnvironment const& env);

} // namespace tests
