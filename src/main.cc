#include "./simhash.hpp"
#define _GNU_SOURCE
using namespace std;

int main(int argc, char **argv){
  //フラグ類
  //d_flag : debug

  bool debug_flag = false, make_hash_flag = false, search_flag = false;
  char *input_feature_name = NULL, *input_hash_name = NULL, *query = NULL;
  int result = 0, option_index = 0;

  struct option lngopt[] = {
    {"hash", 1, NULL, 0},
    {"feature", 1, NULL, 0},
    {"query", 1, NULL, 0},
    {"make", 0, NULL, 0},
    {"search", 0, NULL, 0},
    {0, 0, 0, 0}
  };

  while(1){
    result = getopt_long(argc, argv, "f:h:q:ds", lngopt, &option_index);
    if(result == -1) break;
    if(result == 0){
      switch(option_index){
      case 0 : input_hash_name = optarg; break;
      case 1 : input_feature_name = optarg; break;
      case 2 : query = optarg; break;	
      case 3 : make_hash_flag = true; break;
      case 4 : search_flag = true; break;
      }
    }else{
      switch(result){
      case 'f' : input_feature_name = optarg; break;
      case 'h' : input_hash_name = optarg; break;
      case 'q' : query = optarg; break;
      case 'd' : debug_flag = true; break;
      case 's' : search_flag = true; break;
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
  if(search_flag){
    if(input_hash_name == NULL){
      if(make_hash_flag){
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
  
  if(make_hash_flag){
    cout << "Running make hash mode." << endl;
    SimHash sh;
    sh.set_data_from_file(input_feature_name);
    sh.set_hash_table_from_feature_table();
    sh.output_hash_table();
  }
}
