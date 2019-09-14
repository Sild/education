#define BOOST_TEST_MODULE allocator_tests_module

#include "allocator.h"
#include "container.h"
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(allocator_test_suite)

BOOST_AUTO_TEST_CASE(test_mock) {
    BOOST_ASSERT(1 == 1);
}

BOOST_AUTO_TEST_SUITE_END()
