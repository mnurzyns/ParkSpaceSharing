#include "Assert.hh"

#include <oatpp/web/protocol/http/incoming/Response.hpp>

#include <functional>
#include <iostream>
#include <memory>
#include <optional>
#include <source_location>

namespace tests {

void
testAssert(bool value,
           std::optional<AssertMessageT> message,
           std::source_location const loc)
{
    if (!value) {
        std::cerr << "ASSERT FAILED: " << loc.file_name() << ":" << loc.line()
                  << ":" << loc.column() << " " << loc.function_name() << '\n';
        if (message) {
            (*message)(std::cerr) << '\n';
        }
        exit(1);
        // throw std::runtime_error{"ASSERT FAILED"};
    }
}

/*
  Wrap response into object which can be printed by testAssert.
*/
AssertMessageT
assertWrap(
  std::shared_ptr<oatpp::web::protocol::http::incoming::Response> response)
{
    // Important is that we execute readBodyToString() *only* inside
    // testAssert(). That is why AssertMessageT is supposed to be a function.
    return
      [response = std::move(response)](std::ostream& ostream) -> std::ostream& {
          ostream << *response->readBodyToString();
          return ostream;
      };
}

} // namespace tests
