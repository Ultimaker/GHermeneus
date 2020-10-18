//
// Created by Jelle Spijker on 9/12/20.
//

#include <catch2/catch.hpp>

#include "GHermeneus/Primitive.h"
using primitive_double = GHermeneus::Primitive<double>;

#include <spdlog/spdlog.h>
constexpr spdlog::level::level_enum log_level = spdlog::level::critical;

TEST_CASE("Relational constexpr", "[primitive]")
{
    spdlog::set_level(log_level);
    using primitive = GHermeneus::Primitive<double>;

    constexpr primitive p1{ 1 };
    constexpr primitive p2{ 2 };
    constexpr primitive p3{ 1 };

    SECTION("Equal")
    {
        STATIC_REQUIRE(p1 != p2);
        STATIC_REQUIRE(p1 == p3);
        STATIC_REQUIRE(p1 == p1);
    }

    SECTION("Equal different type")
    {
        STATIC_REQUIRE(p1 != 1.1);
        STATIC_REQUIRE(1.1 != p1);
        STATIC_REQUIRE(p1 == 1.);
        STATIC_REQUIRE(1. == p1);
    }

    SECTION("Greater then")
    {
        STATIC_REQUIRE(p1 < p2);
        STATIC_REQUIRE(p1 <= p3);
        STATIC_REQUIRE(p2 > p1);
        STATIC_REQUIRE(p2 >= p2);
    }

    SECTION("Greater the different type")
    {
        STATIC_REQUIRE(p1 < 10.);
        STATIC_REQUIRE(p1 <= 1.);
        STATIC_REQUIRE(10. > p1);
    }
}

TEST_CASE("Arithmetic constexpr", "[primitive]")
{
    spdlog::set_level(log_level);
    SECTION("Add")
    {
        constexpr primitive_double p1{ 1 };
        constexpr primitive_double p2{ 2 };
        constexpr primitive_double p_res = p1 + p2;

        STATIC_REQUIRE(p_res == 3);
    }

    SECTION("Subtract")
    {
        constexpr primitive_double p1{ 1 };
        constexpr primitive_double p2{ 2 };
        constexpr primitive_double p_res = p1 - p2;

        STATIC_REQUIRE(p_res == -1);
    }
}
