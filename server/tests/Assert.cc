#include "Assert.hh"

#include <oatpp/web/protocol/http/incoming/Response.hpp>

#include <functional>
#include <iostream>
#include <memory>
#include <optional>
#include <source_location>

namespace tests {

std::atomic<int> g_failed_non_critical = 0;

void
testAssert(bool value,
           std::optional<AssertMessageT> message,
           std::source_location const loc)
{
    if (!value) {
        std::cerr << "\x1b[31mASSERT FAILED\x1b[39m: " << loc.file_name() << ":"
                  << loc.line() << ":" << loc.column() << " "
                  << loc.function_name() << '\n';
        if (message) {
            (*message)(std::cerr) << '\n';
        }
        throw AssertFailedError{};
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

void
deferFailure(std::function<void()> const& func)
{
    try {
        func();
    } catch (AssertFailedError const& err) {
        g_failed_non_critical += 1;
    }
}

void
assertDeferredFailures()
{
    if(g_failed_non_critical > 0) {
        std::cerr << "\x1b[31mNumber of deferred tests failed\x1b[39m: " << g_failed_non_critical << "\n";
        exit(EXIT_FAILURE);
    }
}

} // namespace tests
