#pragma once

#include <oatpp/web/protocol/http/incoming/Response.hpp>

#include <functional>
#include <iostream>
#include <memory>
#include <optional>
#include <ostream>
#include <source_location>

namespace tests {

using AssertMessageT = std::function<std::ostream&(std::ostream&)>;
/*
  Asserts supplied value and yells at you if it's false.

  When @p value is false, prints detailed location of the function call, prints
  the message supplied in @p message if it's present, and terminates the
  program.

  @param message Supposed to be obtained from calling assertWrap().

  @note I prefer not to use OATPP_ASSERT(), because it does not give any usefull
  feedback on where it failed.
*/
void
testAssert(bool value,
           std::optional<AssertMessageT> message = {},
           std::source_location loc = std::source_location::current());

/*
  Wrap response into object which can be printed by testAssert.
*/
AssertMessageT
assertWrap(
  std::shared_ptr<oatpp::web::protocol::http::incoming::Response> response);

} // namespace tests
