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
    unint f_id = atoi( param[0].c_str() );
    feature_map[make_pair(d_id, f_id)] = atof(param[1].c_str());
    each_feature_list[d_id].push_back(f_id);
    feature_list.push_back(f_id);
  }
}

double SimHash::get_feature(unint d_id, unint f_id){
  if(feature_map.end() != feature_map.find(make_pair(d_id, f_id))){
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

unint SimHash::convert_data_to_hash(const vector<pair<unint, double> >& data){
  int v[64] = {0};
  for(vector<pair<unint, double> >::const_iterator f = data.begin(); f != data.end(); ++f){
    unint h = (*f).first;
    for(int i = 0; i < 64; ++i){
      if(h & 1){
	v[i] += (*f).second;
      }else{
	v[i] -= (*f).second;
      }
      h = h >> 1;
    }
  }

  for(int x = 63; x >= 0; --x){cout << v[x];}
  cout << endl;
  unint h_value = 0;
  for(int i = 63; i >= 0; --i){
    if(v[i] >= 0){
      h_value = (h_value << 1) | 1;
    }else{
      h_value = (h_value << 1);
    }
  }
  return h_value;
}
