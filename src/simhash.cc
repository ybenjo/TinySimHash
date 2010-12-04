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
  
  //debug mode
  if(debug_flag && d[0] == str){
    cout << "Invalid format : "
	 << str << endl;
    exit(1);
  }
  
  unint d_id = atoi(d[0].c_str());
  for(vector<string>::iterator i = d.begin() + 1; i != d.end(); ++i){
    vector<string> param = split_string(*i, ":");

    //debug mode
    if(debug_flag && param[0] == *i){
      cout << "Invalid format : "
	   << param[0] << endl;
      exit(1);
    }
    
    unint f_id = atoi( param[0].c_str() );
    feature_table[d_id].push_back( make_pair(f_id, atof(param[1].c_str())) );
  }
}

void SimHash::set_data_from_file(char* input_file_name){
  ifstream ifs;
  string tmp;
  vector<string> ret_str;
  this->input_file_name = input_file_name;
  ifs.open(this->input_file_name, ios::in);
  while(ifs && getline(ifs, tmp)){
    set_one_data(tmp);
  }
  ifs.close();
}

void SimHash::set_hash_table_from_file(char* input_file_name){
  ifstream ifs;
  string tmp;

  vector<string> ret_str;
  ifs.open(input_file_name, ios::in);
  while(ifs && getline(ifs, tmp)){
    vector<string> words;
    stringstream ss(tmp);
    string elem;
    while (ss >> elem){
      words.push_back(elem);
    }
    hash_table.push_back(make_pair(atoi(words[0].c_str()), atoi(words[1].c_str())));
  }
  ifs.close();
}


void SimHash::initialize(){
  set_debug_flag(false);
  srand(time(0));
  input_file_name = NULL;
}


void SimHash::output_hash_table(){
  ostringstream oss;
  vector<string> ret = split_string(input_file_name, ".");
  if(ret.size() > 1){
    for(int i = 0; i < ret.size() - 1; ++i){
      oss << ret[i];
    }
  }else{
    oss << ret[0];
  }
  oss << ".hash";
  
  ofstream ofs;
  ofs.open((oss.str()).c_str());
  for(vector<pair<unint, unint> >::iterator i = hash_table.begin(); i != hash_table.end(); ++i){
    ofs << (*i).first << " " << (*i).second << endl;
  }
  ofs.close();
}


//0 以上 limit 未満の乱数取得
unint SimHash::get_random(unint limit = 0){
  if(limit == 0){
    return rand() / (RAND_MAX / (PRIME));
  }else{
    return rand() / (RAND_MAX / (limit));
  }
}

//v のビット列をシャッフル(を近似)
unint SimHash::bit_shuffle(unint v, unint a, unint b, unint p){
  unint this_p = p;
  if(p == 0){this_p = PRIME;}
  return (a*v + b) % this_p;
}

unint SimHash::convert_data_to_hash(const vector<pair<unint, double> >& data){
  //この v[i] の正負が最終的なハッシュ値におけるiビット目の1/0になる
  //とりあえず uint32_t で決め打ちしてるので要素数32で初期化
  int v[sizeof(unint)*8] = {0};
  for(vector<pair<unint, double> >::const_iterator f = data.begin(); f != data.end(); ++f){
    //特徴の id をそのまま特徴に対するハッシュとして使う
    //あとはそれから1ビットずつ取得
    unint h = (*f).first;
    for(int i = 0; i < sizeof(unint)*8; ++i){
      if(h & 1){
	v[i] += (*f).second;
      }else{
	v[i] -= (*f).second;
      }
      h = h >> 1;
    }
  }

  unint h_value = 0;
  //vの後ろからビットを左シフトで押しこんでいく
  for(int i = sizeof(unint)*8 - 1; i >= 0; --i){
    if(v[i] >= 0){
      //左シフトさせて一番下のビットを立てる
      h_value = (h_value << 1) | 1;
    }else{
      //左シフトさせるだけ
      h_value = (h_value << 1);
    }
  }
  return h_value;
}
 
void SimHash::set_hash_table_from_feature_table(){
  unordered_map<unint, vector<pair<unint, double> > >::iterator i;
  for(i = feature_table.begin(); i != feature_table.end(); ++i){
    unint d_id = i->first;
    hash_table.push_back(make_pair(d_id, convert_data_to_hash(i->second)));
  }
}

unint SimHash::set_query_to_hash_table(char* input_query_name){
  unint d_id = PRIME;
  ifstream ifs;
  string tmp;

  vector<pair<unint, double> > features;
  //tmp => param_id : param_score \s param_id : param_score...
  ifs.open(input_query_name, ios::in);
  while(ifs && getline(ifs, tmp)){
    vector<string> d = split_string(tmp, " ");
    
    //debug mode
    if(debug_flag && d[0] == tmp){
      cout << "Invalid format : "
	   << tmp << endl;
      exit(1);
    }

    for(vector<string>::iterator i = d.begin(); i != d.end(); ++i){
      vector<string> param = split_string(*i, ":");
      
      //debug mode
      if(debug_flag && param[0] == *i){
	cout << "Invalid format : "
	     << param[0] << endl;
	exit(1);
      }
      unint f_id = atoi( param[0].c_str() );
      double f_value = atof( param[1].c_str() );
      //set feature and hash-value
      query_feature[f_id] = f_value;
      features.push_back(make_pair(f_id, f_value));
    }
    
  }
  ifs.close();

  hash_table.push_back(make_pair(d_id, convert_data_to_hash(features)));
  return convert_data_to_hash(features);
}

void SimHash::hash_table_bit_shuffle(){
  unint a = get_random();
  unint b = get_random();
  vector<pair<unint, unint> >::iterator i;
  for(i = hash_table.begin(); i != hash_table.end(); ++i){
    (*i).second = bit_shuffle((*i).second, a, b);
  }
}

void SimHash::hash_table_sort(){
  sort(hash_table.begin(), hash_table.end(), pairless());
}
