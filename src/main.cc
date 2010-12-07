#include "./simhash.hpp"
#define _GNU_SOURCE
using namespace std;

int main(int argc, char **argv){
  //フラグ類
  //d_flag : debug

  bool debug_flag = false, make_hash_flag = false, search_flag = false;
  
  char *input_feature_name = NULL, *input_hash_name = NULL,
    *query = NULL, *feature_server_address = NULL, *hash_server_address = NULL;
  
  int result = 0, option_index = 0, iteration = 1, limit = 10;
  unint near_b = 10;

  struct option lngopt[] = {
    {"hash", 1, NULL, 0},
    {"feature", 1, NULL, 0},
    {"query", 1, NULL, 0},
    {"make", 0, NULL, 0},
    {"fserver", 0, NULL, 0},
    {"hserver", 0, NULL, 0},
    {"iteration", 0, NULL, 0},
    {"limit", 0, NULL, 0},
    {0, 0, 0, 0}
  };

  while(1){
    result = getopt_long(argc, argv, "f:h:q:b:i:ds", lngopt, &option_index);
    if(result == -1) break;
    if(result == 0){
      switch(option_index){
      case 0 : input_hash_name = optarg;break;
      case 1 : input_feature_name = optarg; break;
      case 2 : query = optarg; break;	
      case 3 : make_hash_flag = true; break;
      case 4 : feature_server_address = optarg; break;
      case 5 : hash_server_address = optarg; break;
      case 6 : iteration = atoi(optarg); break;
      case 7 : limit = atoi(optarg); break;
      }
    }else{
      switch(result){
      case 'f' : input_feature_name = optarg; break;
      case 'h' : input_hash_name = optarg; break;
      case 'q' : query = optarg; break;
      case 'd' : debug_flag = true; break;
      case 'b' : near_b = atoi(optarg); break;
      case 'i' : iteration = atoi(optarg); break;
      case 'l' : limit = atoi(optarg); break;
      }
    }
    optarg = NULL; 
  }

  //特徴ファイル不在
  if(input_feature_name == NULL){
    cout << "Please specify feature filename." << endl;
    cout << "  ./simhash -f [path] or --feature [path]" << endl;
    exit(1);
  }

  //検索モードにおける例外

  if(!search_flag && query != NULL){
    search_flag = true;
  }

  if(search_flag){
    if(input_hash_name == NULL){
      if(!make_hash_flag){
	cout << "Please specify hash table filename." << endl;
	cout << "  ./simhash --hash [path] or -h [path]" << endl;
	exit(1);
      }else{
	cout << "Please make hash table." << endl;
	cout << "  ./simhash --make" << endl;
	exit(1);
      }
    }else if(query == NULL){
      cout << "Please specify query filename." << endl;
      cout << "  ./simhash --query [query path] or ./simhash -q [query path]" << endl;
      exit(1);
    }
  }



  SimHash sh;
  sh.set_debug_flag(debug_flag);

  
  if(make_hash_flag){
    cout << "Running make hash mode." << endl;
    sh.set_data_from_file(input_feature_name);
    sh.set_hash_table_from_feature_table();
    sh.save_hash_table_to_file();
  }
  
  if(search_flag){
    cout << "Running search mode." << endl;
    sh.set_data_from_file(input_feature_name);
    sh.set_hash_table_from_file(input_hash_name);
    sh.set_query_to_hash_table(query);
    
    for(int i = 0; i < iteration; ++i){
      sh.hash_table_bit_shuffle();
      sh.hash_table_sort();
      sh.search_b_nearest_data(near_b);
    }

    sh.calc_b_nearest_cosine_distance(near_b);
    sh.save_near_cosines_to_file(limit);
  }
}
