//
// Created by fancy on 2020/1/4.
//

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
//#define BOOST_TEST_MAIN - don't need to repeat this define in more than one cpp file
#include <boost/test/unit_test.hpp>

#include "SandStormPlayer.hpp"
BOOST_AUTO_TEST_SUITE(AbsoluteDateCheckSuite)

    BOOST_AUTO_TEST_CASE(ExampleDate)
    {
        SandStormPlayer player;

        BOOST_CHECK_EQUAL(  player.parse("", 0), 0);
        BOOST_CHECK_EQUAL(player.id, 0);
    }

    BOOST_AUTO_TEST_CASE(IncorrectDate)
    {
        const char  sample [] = "0\t | Observer\t\t | INVALID\t\t | \t\t | 0\t\t | 0\t | Commander\t | INVALID\t\t | \t\t | 10\t\t | 0\t | Breacher\t\t | INVALID\t\t | \t\t | 0\t\t | 0\t | Advisor\t\t | INVALID\t\t | \t\t | 0\t\t | 0\t | Demolitions\t | INVALID\t\t | \t\t | 0\t\t | 0\t | Gunner\t\t | INVALID\t\t | \t\t | 0\t\t | ";
        SandStormPlayer player;

        BOOST_CHECK_EQUAL(  player.parse(sample, 37), 0);
        BOOST_CHECK_EQUAL(player.id, 0);
        BOOST_CHECK_EQUAL(player.name, "Observer");
        BOOST_CHECK_EQUAL(player.net_id, "INVALID");
        BOOST_CHECK_EQUAL(player.ip, "");
        BOOST_CHECK_EQUAL(player.score, 0);


    }
BOOST_AUTO_TEST_SUITE_END()