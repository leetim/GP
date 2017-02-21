#include <gtest/gtest.h>
#include <gmock/gmock.h>
// #include <myVector.h>
// #include <myAllocator.h>
#include <iostream>

class TestTest : public ::testing::Test
{
protected:
	void SetUp()
	{
	}
	void TearDown()
	{
	}
};

TEST_F(TestTest, first_Test){
    ASSERT_TRUE(true);
}

TEST_F(TestTest, second_Test){
  ASSERT_TRUE(true);
}

int main(int argc, char* argv[]){
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
