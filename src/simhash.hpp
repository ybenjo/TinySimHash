#ifndef __class__SimHash__
#define __class__SimHash__

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <unordered_map>
#include <set>
#include <vector>
#include <string>
#include <algorithm>
#include <stdlib.h>
#include <math.h>

using namespace std;

//unordered_map に pair を使うための関数オブジェクト
struct myeq : std::binary_function<pair<int, int>, pair<int, int>, bool>{
  bool operator() (const pair<int, int> & x, const pair<int, int> & y) const{
    return x.first == y.first && x.second == y.second;
  }
};

struct myhash : std::unary_function<pair<int, int>, size_t>{
private:
  const hash<int> h_int;
public:
  myhash() : h_int() {}
  size_t operator()(const pair<int, int> & p) const{
    //http://stackoverflow.com/questions/738054/hash-function-for-a-pair-of-long-long
    size_t seed = h_int(p.first);
    return h_int(p.second) + 0x9e3779b9 + (seed<<6) + (seed>>2);
  }
};

//pair の second で sort するための関数オブジェクト
struct myless : std::binary_function<pair<int, int> , pair<int, int>, bool>{
  bool operator()(const pair<int, int> & x, const pair<int, int> y) const {
    return x.second < y.second;
  }
};

class SimHash{
public:
  SimHash(){}
  vector<string> split_string(const string& s, const string& c);
  void set_one_data(string str);
private:
  unordered_map<pair<int, int>, double> feature_map;
};

#endif //__class__SimHash__
