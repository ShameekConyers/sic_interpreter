#include <gtest/gtest.h>


TEST(b, t)
{
  ASSERT_EQ(0, 0);
}

int main(int argc, char** argv)
{
  printf("fj");

  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
