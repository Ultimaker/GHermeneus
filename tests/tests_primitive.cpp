//
// Created by Jelle Spijker on 10/17/20.
//
#include <catch2/catch.hpp>

#include <type_traits>

#include "GHermeneus/Primitive.h"

TEST_CASE("Casting", "[primitive]")
{
    using primitive_float = GHermeneus::Primitive<float>;
    using primitive_double = GHermeneus::Primitive<double>;
    using primitive_long_double = GHermeneus::Primitive<long double>;
    SECTION("Implicit float cast")
    {
        float value = primitive_float{1.F};
        REQUIRE(value == 1.F);
    }

    SECTION("Implicit double cast")
    {
        double value = primitive_double{1.};
        REQUIRE(value == 1.);
    }

    SECTION("Implicit long double cast")
    {
        long double value = primitive_long_double{1.L};
        REQUIRE(value == 1.L);
    }

    SECTION("Static float cast")
    {
        auto value = primitive_float { 1.F };
        auto casted_value = static_cast<float>(value);
        REQUIRE(casted_value == 1.F);
    }

    SECTION("Static double cast")
    {
        auto value = primitive_double { 1 };
        auto casted_value = static_cast<double>(value);
        REQUIRE(casted_value == 1);
    }

    SECTION("Static long double cast")
    {
        auto value = primitive_long_double { 1.L };
        auto casted_value = static_cast<long double>(value);
        REQUIRE(casted_value == 1.L);
    }
}

TEST_CASE("Relational", "[primitive]")
{
    using primitive = GHermeneus::Primitive<double>;

    primitive p1 { 1 };
    primitive p2 { 2 };
    primitive p3 { 1 };

    SECTION("Equal")
    {
        REQUIRE(p1 != p2);
        REQUIRE(p1 == p3);
        REQUIRE(p1 == p1);
    }

    SECTION("Equal different type")
    {
        REQUIRE(p1 != 1.1);
        REQUIRE(1.1 != p1);
        REQUIRE(p1 == 1.);
        REQUIRE(1. == p1);
    }

    SECTION("Greater then")
    {
        REQUIRE(p1 < p2);
        REQUIRE(p1 <= p3);
        REQUIRE(p2 > p1);
        REQUIRE(p2 >= p2);
    }

    SECTION("Greater the different type")
    {
        REQUIRE(p1 < 10.);
        REQUIRE(p1 <= 1.);
        REQUIRE(10. > p1);
    }
}

TEST_CASE("Arithmetic", "[primitive]")
{
    using primitive = GHermeneus::Primitive<double>;
    SECTION("Subtract same types")
    {
        primitive p1 { 1 };
        primitive p2 { 2 };
        auto p_res = p1 - p2;

        REQUIRE(p_res == 1 - 2);

        p_res -= p1;
        REQUIRE(p_res == 1 - 2 - 1);
    }

    SECTION("Subtract different types")
    {
        primitive p1 { 1 };
        primitive p2 { 2 };
        auto p_res = p1 - 2;

        REQUIRE(p_res == 1 - 2);

        p_res -= p1;
        REQUIRE(p_res == 1 - 2 - 1);
    }
}
