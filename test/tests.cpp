#include <catch2/catch.hpp>

#include <ctime>

#include "../src/median.hpp"

TEST_CASE("without numbers", "[median]")
{
    Median m;

    REQUIRE(m.get_median() == 0);
}

TEST_CASE("2 equal numbers", "[median]")
{
    Median m;

    m.add(3);
    m.add(3);

    REQUIRE(m.get_median() == 3);
}

TEST_CASE("3 numbers", "[median]")
{
    Median m;

    m.add(1);
    m.add(3);
    m.add(5);

    REQUIRE(m.get_median() == 3);
}

TEST_CASE("3 numbers, 1 double", "[median]")
{
    Median m;

    m.add(1);
    m.add(1.3);
    m.add(100);

    REQUIRE(m.get_median() == 1.3);
}

TEST_CASE("add 1000 numbers", "[median]")
{
    Median m;

    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    for(size_t i = 0; i < 1000; ++i)
    {
        m.add(std::rand());
    }
}
