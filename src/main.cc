#include "./simhash.hpp"
#define _GNU_SOURCE
using namespace std;

int main(int argc, char **argv){
  //フラグ類
  //d_flag : debug

  bool debug_flag = false, make_hash_flag = false,
    search_flag = false, fast_flag = false, normal_flag = false;
  
  char *input_feature_name = NULL, *input_hash_name = NULL, *query = NULL,
    *feature_server_address = NULL, *hash_server_address = NULL;
  
  int result = 0, option_index = 0, iteration = 1, limit = 10, k = 34;
    
  
  unint near_b = 10;

  struct option lngopt[] = {
    {"hash", 1, NULL, 0},
    {"feature", 1, NULL, 0},
    {"query", 1, NULL, 0},
    {"make", 0, NULL, 0},
    {"fserver", 1, NULL, 0},
    {"hserver", 1, NULL, 0},
    {"iteration", 1, NULL, 0},
    {"limit", 1, NULL, 0},
    {"fast", 0, NULL, 0},
    {"normal", 0, NULL, 0},
    {0, 0, 0, 0}
  };

  while(1){
    result = getopt_long(argc, argv, "f:h:q:b:i:k:ds", lngopt, &option_index);
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
      case 8 : fast_flag = true; break;
      case 9 : normal_flag = true; break;
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
      case 'k' : k = atoi(optarg); break;
      }
    }
    optarg = NULL; 
  }

  //検索モードにおける例外
  if(!search_flag && query != NULL){
    search_flag = true;
  }

  if(search_flag){
    if(input_hash_name == NULL && hash_server_address == NULL){
      if(!make_hash_flag){
	cout << "Please specify hash table filename or hash table server's address. \n"
	     << "  ./simhash --hash [path] or -h [path]\n"
	     << "or\n"
	     << "  ./simhash --hserver [path] (ex. --hserver localhost:1978)" << endl;
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

  //特徴ファイル不在
  if(search_flag && input_feature_name == NULL && feature_server_address == NULL){
    cout << "Please specify feature filename or feature server's address.\n"
	 << "  ./simhash -f [path] or --feature [path]\n"
	 << "or"
	 << " ./simhash --fserver [path] (ex. --fserver localhost:1978)" << endl;
    exit(1);
  }else if(!search_flag && input_feature_name == NULL){
    cout << "Please specify feature filename or feature server's address.\n"
	 << "  ./simhash -f [path] or --feature [path]" << endl;
  }
  


  SimHash sh;
  sh.set_debug_flag(debug_flag);
  
  if(make_hash_flag){
    cout << "Running make hash mode." << endl;
    sh.set_data_from_file(input_feature_name);
    sh.set_hash_table_from_feature_table();

    if(feature_server_address != NULL){
      sh.save_feature_to_tt(feature_server_address);
    }

    if(fast_flag){
      //fast
      sh.save_split_hash_table_to_tt(hash_server_address);
    }else{
      //normal
      if(hash_server_address != NULL){
	sh.save_hash_table_to_tt(hash_server_address);
      }else{
	sh.save_hash_table_to_file();
      }
    }
  }
  
  if(search_flag){
    cout << "Running search mode." << endl;
    sh.set_query_to_hash_table(query);
    sh.query_normalization(normal_flag);

    if(fast_flag){
      //この場合、ハッシュを4分割し、そのテーブルごとに一致するハッシュを取得する。
      sh.get_split_hash_table_to_tt(hash_server_address);
      sh.unique_near_ids();
      if(k != 34){
	sh.bit_xor(k);
      }
    }else{

      if(hash_server_address != NULL){
	sh.get_hash_table_from_tt(hash_server_address);
      }else{
	sh.set_hash_table_from_file(input_hash_name);
      }
      //こちらは全件hashを読み込む
      //normal simhash (random shuffle and sort)
    
      for(int i = 0; i < iteration; ++i){
	sh.hash_table_bit_shuffle();
	sh.hash_table_sort();
	sh.search_b_nearest_data(near_b);
      }
      
      sh.unique_near_ids();
    }
    
    if(feature_server_address != NULL){
      sh.get_feature_from_tt(feature_server_address);
    }else{
      sh.set_data_from_file(input_feature_name);
    }
    
    sh.calc_b_nearest_cosine_distance(near_b);
    sh.save_near_cosines_to_file(limit);
  }
}
