#define BOOST_TEST_MODULE ip_filter_tests_module

#include "libs.h"

#include <limits>
#include <string>
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(ip_filter_tests_suite)

BOOST_AUTO_TEST_CASE(test_stoui) {
    std::string good_given = "5";
    BOOST_ASSERT(5 == stoui(good_given));
    std::string bad_given ="999999999999";
    try {
        stoui(bad_given);
        BOOST_ASSERT(false);
    } catch (const std::bad_cast&) {
    }
}

BOOST_AUTO_TEST_CASE(test_split) {
    auto given = split("1-2-3", '-');
    auto expected = std::vector<std::string>({"1", "2", "3"});
    BOOST_CHECK_EQUAL_COLLECTIONS(expected.begin(), expected.end(), given.begin(), given.end());
}

BOOST_AUTO_TEST_CASE(test_sort) {
    using namespace std;
    using ips = vector<vector<uint32_t>>;
    ips given = {{0, 0, 0, 0}, {1, 1, 1, 1}, {2, 2, 2, 2}};
    ips expected = {{2, 2, 2, 2}, {1, 1, 1, 1}, {0, 0, 0, 0}};
    sort(given);
    for(size_t i = 0; i < given.size(); ++i) {
        BOOST_CHECK_EQUAL_COLLECTIONS(expected.at(i).begin(), expected.at(i).end(), given.at(i).begin(), given.at(i).end());
    }
}

BOOST_AUTO_TEST_CASE(test_filter_accept_all) {
    BOOST_ASSERT(filter_accept_all(123));
}

BOOST_AUTO_TEST_CASE(test_filter_accept_check_first) {
    std::vector<uint32_t> accepted = {1, 0, 3};
    std::vector<uint32_t> failed = {0, 1, 2};
    auto cff = filter_seq<std::vector<uint32_t>, 1>;
    BOOST_ASSERT(cff(accepted));
    BOOST_ASSERT(!cff(failed));
}

BOOST_AUTO_TEST_CASE(test_filter_accept_check_first_second) {
    std::vector<uint32_t> accepted = {46, 70, 3};
    std::vector<uint32_t> failed1 = {46, 53, 2};
    std::vector<uint32_t> failed2 = {47, 70, 2};
    std::vector<uint32_t> failed3 = {90, 90, 2};
    auto cfsf = filter_seq<std::vector<uint32_t>, 46, 70>;
    BOOST_ASSERT(cfsf(accepted));
    BOOST_ASSERT(!cfsf(failed1));
    BOOST_ASSERT(!cfsf(failed2));
    BOOST_ASSERT(!cfsf(failed3));
}

BOOST_AUTO_TEST_CASE(test_seq_too_long) {
    std::vector<uint32_t> short_array = {46};
    try {
        filter_seq<std::vector<uint32_t>, 46, 70>(short_array);
        BOOST_ASSERT(false);
    } catch (const std::runtime_error& e) {
    }
}

BOOST_AUTO_TEST_CASE(test_filter_accept_check_any) {
    std::vector<uint32_t> accepted1 = {46, 70, 3};
    std::vector<uint32_t> accepted2 = {2, 46, 3};
    std::vector<uint32_t> accepted3 = {1, 19, 46};
    std::vector<uint32_t> fail = {1, 19, 23};
    BOOST_ASSERT(filter_accept_check_any(accepted1));
    BOOST_ASSERT(filter_accept_check_any(accepted2));
    BOOST_ASSERT(filter_accept_check_any(accepted3));
    BOOST_ASSERT(!filter_accept_check_any(fail));
}

BOOST_AUTO_TEST_SUITE_END()
