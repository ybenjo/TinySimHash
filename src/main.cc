#include "./simhash.hpp"

int main(int argc, char **argv){
  //フラグ類
  //d_flag : debug

  bool debug_flag = false;
  char *input_file_name = NULL, *output_file_name = NULL;
  int result = 0;

  while(1){
    result = getopt(argc, argv, "i:d");
    if(result == -1) break;
    switch(result){
    case 'i' : input_file_name = optarg; break; 
    case 'd' : debug_flag = true; break;
    }
    optarg = NULL; 
  }

  SimHash sh;
  sh.set_data_from_file(input_file_name);
  sh.set_hash_table_from_feature_table();
}
