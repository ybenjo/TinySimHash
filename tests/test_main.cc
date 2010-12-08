#include <gtest/gtest.h>
#include "../src/simhash.hpp"

using namespace std;

class SimHashModule : public ::testing::Test{
protected:
  void SetUp(){ 
  }
  void TearDown(){
  }
  SimHash sh;
};


TEST_F(SimHashModule, split_string_case){
  string str = "123 456";
  vector<string> ret = sh.split_string(str, " ");
  EXPECT_EQ("123", ret[0]);
  EXPECT_EQ("456", ret[1]);
}

TEST_F(SimHashModule, split_string_case_empty_string){
  string str = " ";
  vector<string> ret = sh.split_string(str, " ");
  EXPECT_EQ(2, ret.size());
  EXPECT_EQ("", ret[0]);
  EXPECT_EQ("", ret[1]);
}

TEST_F(SimHashModule, split_string_case_no_delimiter){
  string str = "1234567890";
  vector<string> ret = sh.split_string(str, ":");
  EXPECT_EQ("1234567890", ret[0]);
}

TEST_F(SimHashModule, bit_shuffle){
  EXPECT_EQ(68, sh.bit_shuffle(21, 3, 5));
}

TEST_F(SimHashModule, convert_data_to_hash_simple_case1){
  vector<pair<unint, double> > data;
  data.push_back(make_pair(0, 1));
  EXPECT_EQ(0, sh.convert_data_to_hash(data) );
}

TEST_F(SimHashModule, convert_data_to_hash_simple_case2){
  vector<pair<unint, double> > data;
  data.push_back(make_pair(8, 1));
  EXPECT_EQ(8, sh.convert_data_to_hash(data) );
}

TEST_F(SimHashModule, convert_data_to_hash_complicated_case){
  vector<pair<unint, double> > data;
  data.push_back(make_pair(1, 2));
  data.push_back(make_pair(2, 3));
  data.push_back(make_pair(3, 4));
  EXPECT_EQ(3, sh.convert_data_to_hash(data) );
}

TEST_F(SimHashModule, set_query_to_hash_table){
  EXPECT_EQ(3, sh.set_query_to_hash_table("./tests/q_test.txt"));
}

TEST_F(SimHashModule, split_number_bit_case1){
  EXPECT_EQ(0, sh.split_number_bit(0, 0, 7));
}

TEST_F(SimHashModule, split_number_bit_case2){
  EXPECT_EQ(1, sh.split_number_bit(256, 0, 7));
}

TEST_F(SimHashModule, split_number_table_1){
  //00001xx + 11 = 0000111
  EXPECT_EQ(7, sh.split_number_table(256, 3));
}

TEST_F(SimHashModule, split_number_table_2){
  //111111xx + 10 = 111111110
  unint unint_max = ~0;
  EXPECT_EQ(67108862, sh.split_number_table(unint_max, 2));
}

int main(int argc, char **argv){
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

