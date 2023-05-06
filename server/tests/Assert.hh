#pragma once

#include <oatpp/web/protocol/http/incoming/Response.hpp>

#include <functional>
#include <iostream>
#include <memory>
#include <optional>
#include <ostream>
#include <source_location>
#include <stdexcept>

namespace tests {

class AssertFailedError : public virtual std::runtime_error
{
  public:
    AssertFailedError()
      : std::runtime_error{ "AssertFailedError" }
    {
    }
};

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

/*
  If the supplied function throws AssertFailedError, instead of tests
  failing immediately, they will fail when calling assertFailed().

  The reasoning behind this is the following:
  When we have a 'tree' of tests that look like this:

  - OfferController
    - GET
     - 200
     - 404
  - PlaceController
   - ...

  In a situation when test 'OfferController->GET->200' fails, we still want to
  execute other tests that follow, especially tests targeting the
  PlaceController which is not affected by the previous failure.
 */
void
deferFailure(std::function<void()> const& func);

void
assertDeferredFailures();

} // namespace tests
