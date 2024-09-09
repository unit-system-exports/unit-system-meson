#include "test_functions.hpp"

#include <iostream>

using namespace sakurajin::unit_system::literals;
using namespace sakurajin::unit_system;

// base units
static const auto t1 = 10.0_s;
static const auto s1 = 250.0_m;
static const auto s2 = 10.0_m;
static const auto m1 = 1_kg;

// common units
static const auto v1 = 25_mps;
static const auto v2 = 1_mps;
static const auto a1 = 2.5_mps2;
static const auto p1 = 25_kgmps;
static const auto F1 = 2.5_N;
static const auto E1 = 25_J;
static const auto P1 = 2.5_W;
static const auto A1 = 100_m2;

TEST(common_tests, speed_test)
{

    const time_si _t1 { 1, 1 };
    const auto _t2 = 10.0_h;

    const length _s1 { s2 };
    const auto _s2 = 250.0_km;

    const speed _v1 = _s1 / _t1;
    EXPECT_UNIT_EQ(_v1, 10.0_mps);

    const auto _v2 = _s2 / _t2;
    EXPECT_UNIT_EQ(_v2, 25.0_kmph);

    const auto _v3 = unit_cast(3.6_kmph, 1);
    EXPECT_UNIT_EQ(_v3, 1.0_mps);

    EXPECT_UNIT_EQ(_v3 * _t1, _t1 * _v3);
}

TEST(common_tests, acceleration_test)
{

    const auto _a1 = v1 / t1;
    EXPECT_UNIT_EQ(_a1, a1);
}

TEST(common_tests, momentum_test)
{

    const auto _p1 = v1 * m1;
    EXPECT_UNIT_EQ(_p1, m1 * v1);
    EXPECT_UNIT_EQ(_p1, p1);
}

TEST(common_tests, force_test)
{

    EXPECT_UNIT_EQ(p1 / 10_s, F1);
    EXPECT_UNIT_EQ(m1 * a1, F1);
}

TEST(common_tests, energy_test)
{

    EXPECT_UNIT_EQ(F1 * s2, E1);
    EXPECT_UNIT_EQ(p1 * v2, E1);
}

TEST(common_tests, power_test)
{

    EXPECT_UNIT_EQ(F1 * v2, P1);
    EXPECT_UNIT_EQ(E1 / t1, P1);
}

TEST(common_tests, area_test)
{

    const auto _A2 = s1 * s2;
    EXPECT_UNIT_EQ(_A2, 2500_m2);

    EXPECT_UNIT_EQ(square(s2), A1);
    EXPECT_UNIT_EQ(A1 / s2, s2);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
