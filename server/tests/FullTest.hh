#pragma once

#include <oatpp-test/UnitTest.hpp>

class FullTest : public oatpp::test::UnitTest
{
  public:
    FullTest()
      : oatpp::test::UnitTest("[FullTest]")
    {
    }

    void
    onRun() override;
};
