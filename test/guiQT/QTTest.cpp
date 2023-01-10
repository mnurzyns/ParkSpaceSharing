#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "guiQT/qt.hpp"

namespace {

    TEST(Qt, returnX)
    {
        ASSERT_EQ(Qt::returnX(3), 3);
    }
}
