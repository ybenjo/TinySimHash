#include "./simhash.hpp"

using namespace std;
using namespace std::tr1;

//デリミタcが存在しない場合、sそのものを返す
vector<string> SimHash::split_string(string s, string c){
 vector<string> ret;
  for(int i = 0, n = 0; i <= s.length(); i = n + 1){
    n = s.find_first_of(c, i);
    if(n == string::npos) n = s.length();
    string tmp = s.substr(i, n-i);
    ret.push_back(tmp);
  }
  return ret;
}

void SimHash::set_one_data(string str){
  //str => d_id \s param_id : param_score
  vector<string> d = split_string(str, " ");
  unint d_id = atoi(d[0].c_str());
  for(vector<string>::iterator i = d.begin() + 1; i != d.end(); ++i){
    vector<string> param = split_string(*i, ":");
    if(debug_flag && param[0] == *i){
      cout << "Invalid format : "
	   << param[0] << endl;
      exit(1);
    }
    unint f_id = atoi(param[0].c_str());
    feature_map[make_pair(d_id, f_id)] = atof(param[1].c_str());
    feature_list.push_back(f_id);
  }
}

double SimHash::get_feature(unint d_id, unint f_id){
  bool exist_1 = feature_map.end() != feature_map.find(make_pair(d_id, f_id));
  if(exist_1){
    return feature_map[make_pair(d_id, f_id)];
  }else{
    return 0.0;
  }
}

void SimHash::initialize(){
  set_debug_flag(false);
  srand(time(0));
}

unint SimHash::get_random(unint limit){
  return rand() / (RAND_MAX / (limit));
}

unint SimHash::bit_shuffle(unint v, unint a, unint b, unint p){
  unint this_p = p;
  if(p == 0){this_p = PRIME;}
  return (a*v + b) % this_p;
}

void SimHash::set_hash_params(){
  sort(feature_list.begin(), feature_list.end());
  feature_list.erase( unique( feature_list.begin(), feature_list.end() ), feature_list.end());
  
  for(vector<unint>::iterator i = feature_list.begin(); i != feature_list.end(); ++i){
    vector<unint> params;
    unint tmp = PRIME;
    params.push_back( get_random(tmp) );
    params.push_back( get_random(tmp) );
    hash_params[*i] = params;
  }
}
