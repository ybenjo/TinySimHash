#ifndef __class__SimHash__
#define __class__SimHash__

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <tr1/unordered_map>
#include <set>
#include <vector>
#include <string>
#include <algorithm>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <ctime>
#include <unistd.h>
#include <getopt.h>

//ハッシュ値は 32ビット
typedef uint32_t unint;
//ついでに素数の最大値を固定しておく
#define PRIME 2147483647

//unordered_map に pair を使うための関数オブジェクト
struct myeq : std::binary_function<std::pair<unint, unint>, std::pair<unint, unint>, bool>{
  bool operator() (const std::pair<unint, unint> & x, const std::pair<unint, unint> & y) const{
    return x == y;
  }
};

//unordered_map に pair を使うための関数オブジェクト
struct myhash : std::unary_function<std::pair<unint, unint>, size_t>{
private:
  const std::tr1::hash<unint> h_int;
public:
  myhash() : h_int() {}
  size_t operator()(const std::pair<unint, unint> & p) const{
    //http://stackoverflow.com/questions/738054/hash-function-for-a-pair-of-long-long
    size_t seed = h_int(p.first);
    return h_int(p.second) + 0x9e3779b9 + (seed<<6) + (seed>>2);
  }
};

//pair の second で sort するための関数オブジェクト
struct pairless : std::binary_function<std::pair<unint, unint> , std::pair<unint, unint>, bool>{
  bool operator()(const std::pair<unint, unint> & x, const std::pair<unint, unint> & y) const {
    return x.second < y.second;
  }
};

class SimHash{
public:
  SimHash(){
    initialize();
  }

  //util
  void initialize();
  std::vector<std::string> split_string(std::string s, std::string c);

  //setter
  void set_debug_flag(bool flag){debug_flag = flag;};
  void set_one_data(std::string str);
  void set_data_from_file(char* input_file_name);
  void set_hash_table_from_feature_table();
  void set_hash_table_from_file(char* input_file_name);
  unint set_query_to_hash_table(char* input_query_name);

  //getter
  unint get_random(unint limit);

  //output
  void output_hash_table();
  
  //calculate
  unint convert_data_to_hash(const std::vector<std::pair<unint, double> >& data);
  unint bit_shuffle(unint v, unint a, unint b, unint p = 0);
  void hash_table_bit_shuffle();
  void hash_table_sort();
  std::vector<unint> search_b_nearest_data(unint b);
  double calculate_cosine_distance(unint d_id_1, unint d_id_2);
  
private:
  std::tr1::unordered_map<unint, std::vector<std::pair<unint, double> > > feature_table;
  std::vector<std::pair<unint, unint> > hash_table;
  std::tr1::unordered_map<unint, double> query_feature;
  unint query_hash;
  bool debug_flag;
  char* input_file_name;
  char* input_query_name;
  unint near_b; 
};

#endif //__class__SimHash__
