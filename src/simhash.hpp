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
typedef uint32_t unint;
#define PRIME 2147483647;

//unordered_map に pair を使うための関数オブジェクト
struct myeq : std::binary_function<std::pair<unint, unint>, std::pair<unint, unint>, bool>{
  bool operator() (const std::pair<unint, unint> & x, const std::pair<unint, unint> & y) const{
    return x == y;
  }
};

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
struct myless : std::binary_function<std::pair<unint, unint> , std::pair<unint, unint>, bool>{
  bool operator()(const std::pair<unint, unint> & x, const std::pair<unint, unint> y) const {
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
  void set_hash_params();
  void set_prime_table();

  //getter
  double get_feature(unint d_id, unint f_id);
  unint get_random(unint limit);

  unint bit_shuffle(unint v, unint a, unint b, unint p = 0);
  
private:
  std::tr1::unordered_map<std::pair<unint, unint>, double, myhash, myeq> feature_map;
  std::tr1::unordered_map<unint, std::vector<unint> > shuffle_params;
  std::tr1::unordered_map<unint, std::vector<unint> > hash_params;
  std::vector<unint> feature_list;
  bool debug_flag;
};

#endif //__class__SimHash__
