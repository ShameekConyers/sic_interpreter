#include "../../src/math/common.hpp"
#include <gtest/gtest.h>
#include <map>
#include <vector>
#include "cmath"

TEST(MATH_COMMON, POWER_TWO_TEST_1)
{
  std::map<int, bool> nums_to_test = {
    {2, true},
    {1, true},
    {1000, false},
    {16, true}
  };

  for (auto& num_pair : nums_to_test) {
    EXPECT_EQ(sic::is_power_of_two(num_pair.first), num_pair.second);
  }

  // EXPECT_ANY_THROW(sic::is_power_of_two("A"));

}

TEST(MATH_COMMON, POWER_TEST_1)
{
  std::vector<int> base_nums_to_test;
  std::vector<int> exponents_to_test;

  for (int i = 0; i < 100; i++) {
    base_nums_to_test.push_back(i);
  }

  for (int i = 0; i < 3; i++) {
    exponents_to_test.push_back(i);
  }

  for (auto exponent : exponents_to_test) {
    for (auto base : base_nums_to_test) {
      auto res = sic::power(base, exponent);
      EXPECT_EQ(res, pow(base, exponent));
    }
  }
}
