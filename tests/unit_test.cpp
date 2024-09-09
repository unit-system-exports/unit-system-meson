#include "test_functions.hpp"

using namespace sakurajin::unit_system;

TEST(unit_t_tests, cast_tests)
{

    const speed v1 { 1000, 1 };
    const speed v2 { 1, 1000 };
    const speed v3 { 10, 100 };
    const speed v4 { 1, 1 };
    const speed v5 { 1, std::kilo::type {} };
    const speed v6 { -1, std::kilo::type {} };

    EXPECT_UNIT_EQ(v1, v2);
    EXPECT_UNIT_EQ(v1, v3);
    EXPECT_UNIT_EQ(v2, v3);
    EXPECT_UNIT_EQ(v2, v5);

    EXPECT_TRUE(v1 != v4);

    EXPECT_UNIT_EQ(std::abs(v5), std::abs(v6));
}

TEST(unit_t_tests, compare_tests)
{

    const speed v1 { 1000, 1 };
    const speed v2 { 1, 1000 };
    const speed v3 { 10, 100 };
    const speed v4 { 1, 1 };
    const speed v5 { 1, std::kilo::type {} };

    EXPECT_TRUE(v4 < v5);
    EXPECT_TRUE(v5 > v4);
    EXPECT_TRUE(v4 <= v5);
    EXPECT_TRUE(v5 >= v4);

    EXPECT_TRUE(v1 <= v1);
    EXPECT_TRUE(v1 >= v1);
}

TEST(unit_t_tests, operator_tests)
{

    const speed v1 { 1000, 1 };
    const speed v2 { 1, 1000 };
    const speed v3 { 10, 100 };
    const speed v4 { 1, 1 };
    const speed v5 { 999, 1 };

    auto _v4 = v4;
    EXPECT_TRUE(v2 != _v4);
    _v4 = v2;
    EXPECT_TRUE(v2 == _v4);

    auto v6 = v1 + v2;
    EXPECT_UNIT_EQ(v6, 2000_mps);
    EXPECT_UNIT_EQ(v6, v1 * 2.0);
    EXPECT_UNIT_EQ(v1, v6 / 2.0);

    auto v7 = v2 + v1;
    EXPECT_UNIT_EQ(v7, 2000_mps);
    EXPECT_UNIT_EQ(v6, v7);

    auto v8 = v1 - v4;
    EXPECT_UNIT_EQ(v5, v8);

    EXPECT_DOUBLE_EQ(v1 / v2, 1.0);

    EXPECT_UNIT_EQ(v1 * 2.0, 2.0 * v1);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
