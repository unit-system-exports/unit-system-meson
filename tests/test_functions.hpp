#include <gtest/gtest.h>

#include "unit_system.hpp"

#define EXPECT_UNIT_EQ(X, Y) EXPECT_DOUBLE_EQ((X).val(), (Y).convert_like(X).val())
