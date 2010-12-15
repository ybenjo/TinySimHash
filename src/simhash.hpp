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

//tokyotyrant
#include <tcrdb.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

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

//pair の second で "降順で" sort するための関数オブジェクト
struct pairlessdouble : std::binary_function<std::pair<unint, double> , std::pair<unint, double>, bool>{
  bool operator()(const std::pair<unint, double> & x, const std::pair<unint, double> & y) const {
    //return x.second < y.second;
    return x.second > y.second;
  }
};

class SimHash{
public:
  SimHash(){
    initialize();
  }

  //util
  void initialize();
  std::vector<std::string> split_string(const std::string& s, const std::string& c);

  //setter
  void set_debug_flag(bool flag){debug_flag = flag;};
  void set_one_data(const std::string& str);
  void set_data_from_file(char* input_file_name);
  void set_hash_table_from_feature_table();
  void set_hash_table_from_line(const std::string& str);
  void set_hash_table_from_file(char* input_file_name);
  unint set_query_to_hash_table(char* input_query_name);

  //getter
  unint get_random(unint limit);

  //output
  void save_hash_table_to_file();
  void save_near_cosines_to_file(int limit);
  
  //calculate
  unint convert_data_to_hash(const std::vector<std::pair<unint, double> >& data);
  unint bit_shuffle(unint v, unint a, unint b, unint p = 0);
  void hash_table_bit_shuffle();
  void hash_table_sort();
  void unique_near_ids();
  void search_b_nearest_data(unint b);
  double calculate_cosine_distance(unint d_id_1, unint d_id_2);
  void calc_b_nearest_cosine_distance(unint b);
  
  //TokyoTyrant
  //setter
  void save_feature_to_tt(char* feature_server_address);
  void save_hash_table_to_tt(char* hash_server_address);

  
  //getter
  void get_feature_from_tt(char* feature_server_address);
  void get_hash_table_from_tt(char* hash_server_address);

  //hashtableの分割
  unint split_number_bit(unint n, unint start, unint end);
  unint split_number_table(unint n, unint table_num);
  void save_split_hash_table_to_tt(char* hash_server_address);
  void get_split_hash_table_to_tt(char* hash_server_address);
  unint bitcount(unint n);
  void bit_xor(int k);
  
private:
  std::tr1::unordered_map<unint, std::vector<std::pair<unint, double> > > feature_table;
  std::vector<std::pair<unint, unint> > hash_table;
  std::tr1::unordered_map<unint, double> query_feature;
  std::vector<unint> near_ids;
  std::vector<std::pair<unint, double> > near_cosines;
  std::tr1::unordered_map<unint, unint> limit_hash_map;
  unint query_hash;
  bool debug_flag;
  char* input_file_name;
  char* input_query_name;
  unint near_b;
  unint q_id;
};

#endif //__class__SimHash__
