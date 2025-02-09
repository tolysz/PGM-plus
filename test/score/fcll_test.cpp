#include <catch2/catch.hpp>
#include <pgm/pgm.h>
#include "../test_utility.h"

TEST_CASE( "fcll" ) {
    // structure
    pgm::Bayesnet bn;
    REQUIRE(bn.add_node("A", {"F", "T"}));
    REQUIRE(bn.add_node("B", {"F", "T"}));
    REQUIRE(bn.add_node("C", {"F", "T"}));
    REQUIRE(bn.add_node("D", {"F", "T"}));
    REQUIRE(bn.add_node("E", {"F", "T"}));

    REQUIRE(bn.add_arc("A", "B"));
    REQUIRE(bn.add_arc("A", "C"));
    REQUIRE(bn.add_arc("B", "D"));
    REQUIRE(bn.add_arc("C", "D"));
    REQUIRE(bn.add_arc("C", "E"));

    // dataset
    pgm::Dataset dataset;
    for (std::size_t i = 0; i < 20; ++i)
        dataset.push({{"A", "T"}, {"B", "F"}, {"C", "T"}, {"D", "T"}, {"E", "T"}});

    for (std::size_t i = 0; i < 15; ++i)
        dataset.push({{"A", "T"}, {"B", "F"}, {"C", "F"}, {"D", "F"}, {"E", "F"}});

    for (std::size_t i = 0; i < 10; ++i)
        dataset.push({{"A", "F"}, {"B", "T"}, {"C", "F"}, {"D", "T"}, {"E", "T"}});

    for (std::size_t i = 0; i < 15; ++i)
        dataset.push({{"A", "F"}, {"B", "F"}, {"C", "T"}, {"D", "T"}, {"E", "T"}});

    for (std::size_t i = 0; i < 5; ++i)
        dataset.push({{"A", "F"}, {"B", "F"}, {"C", "F"}, {"D", "F"}, {"E", "F"}});

    for (std::size_t i = 0; i < 2; ++i)
        dataset.push({{"A", "T"}, {"B", "T"}, {"C", "F"}, {"D", "T"}, {"E", "F"}});

    SECTION( "score output" ) {
        pgm::Fcll score(dataset, "A");

        REQUIRE(equal(score(bn), 1021.4493));
        REQUIRE(equal(score.score("A", {}), -14.0394));
        REQUIRE(equal(score.score("B", {"A"}), 39.5938));
        REQUIRE(equal(score.score("C", {"A"}), 32.1825));
        REQUIRE(equal(score.score("D", {"B", "C"}), 635.3603));
        REQUIRE(equal(score.score("E", {"C"}), 328.3520));
    }
}