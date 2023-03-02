/*
Makes a poster of very many randomly generated sequences and their secondary structure
*/

#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <functional> //for std::function
#include <algorithm>  //for std::generate_n
#include <fstream>

#ifdef __cplusplus
extern "C"
{
#endif

// C header here
#include <cstdlib>


#include <ViennaRNA/fold.h>
#include <ViennaRNA/utils/basic.h>

#include <RNAstruct.h>
#include <dist_vars.h>
#include <treedist.h>

#ifdef __cplusplus
}
#endif

using namespace std;
typedef std::vector<char> char_array;

char_array charset()
{
    //Change this to suit
    return char_array( 
    {'A','U','G','C'});
};    

std::string random_string( size_t length, std::function<char(void)> rand_char )
{
    std::string str(length,0);
    std::generate_n( str.begin(), length, rand_char );
    return str;
}


int main(int argc, char *argv[]) {
  if(argc<3 || argc>3){
    std::cerr << "randRNA: folds RNA sequences into Shapiro str\nUsage: ./randRNA Numseq Lenseq" << '\n';
    exit(1);
  }
  auto N = atoi(argv[1]);
  auto seqlen = atoi(argv[2]);
  // RANDOM NUMBER GEN
  // std::random_device random_device;
  // std::mt19937 random_engine(random_device());
  // std::uniform_int_distribution<int> distribution_1_100(0, 3);
  
  // OTHER INIT
  const auto ch_set = charset();
  std::default_random_engine rng(std::random_device{}());
  std::uniform_int_distribution<> dist(0, ch_set.size()-1);
  auto randchar = [ ch_set,&dist,&rng ](){return ch_set[ dist(rng) ];};
  
  char *b2c;
  static char *target_expand_shapiro;
  char *expand_shapiro;
  
  // auto N = 2000;
  // auto seqlen = 80;
  // 
  ofstream myfile;
  myfile.open ("folded_output.txt");
    
  std::cerr << "How many sequences I have folded: " << '\n';
  for(auto i=0;i<N;++i){
    auto seq = random_string(seqlen,randchar);
    
    char str[seqlen+1];
    float mfe = vrna_fold(seq.c_str(), str);
    b2c=b2C(str);
    expand_shapiro=expand_Shapiro(b2c);
    
    //std::cout << seq<<" "<< str << " "<< expand_shapiro << '\n';
    myfile << seq<<" "<< str << " "<< expand_shapiro << '\n';
        
    free(b2c);
    free(expand_shapiro);
    
    if(i%5000==0) std::cerr << i <<" ";
  }  
  
  myfile.close();
  return 0;
  
  
  // int N=10;
  // int seqlen=30;
  // static const char aplhabet[4]={'A','U','G','C'};
  // std::vector<std::string> v(N);
  // 
  // for(int i=0;i<10; i++){
  //   auto const randomNumber = distribution_1_100(random_engine);
  //   std::cout << randomNumber << '\n';
  // }  
  // return 0;
}