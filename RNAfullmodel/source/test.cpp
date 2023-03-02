#include <vector>
#include <iostream>
#include <string>
extern "C"{
#include <unistd.h>
}

#include "rna_fold_lib.hpp"
#include "p-observer.hpp"
#include "rna_replicator.hpp"
#include "color-manager.hpp"
#include "plot_grace.hpp"

namespace Test {
  char RNAc2cc(char rna);
  void CmplSeq(const std::string &seq,std::string &cseq);
}


extern "C" void test()
{
  RNAFoldLib fold_lib("...(...)...(...)...");
  RNAreplicator rna("AUUAUUGAUGGUCGUUAUAUGCCUAACUGGAUUGAGUAAUCUAGUCUUUU",fold_lib);
  RNAreplicator crna(rna.getCmplSequence(),fold_lib);


  Pobserver pobserver;

  std::vector <int> f_pop;
  std::vector<unsigned int> color;
  std::vector<int> p_pop,m_pop;
  std::vector<bool> if_cat;
  pobserver.getPopulation(f_pop,color,p_pop,m_pop,if_cat);
  
  std::cout << color.size() << std::endl;

  PlotGrace plot_grace;
  plot_grace.plotPhenotype(0,color,p_pop,m_pop,if_cat);
  plot_grace.plotPhenotype(1,color,p_pop,m_pop,if_cat);
  plot_grace.plotFunction(0,f_pop[0],f_pop[1],f_pop[2],f_pop[3],0);
  plot_grace.plotFunction(1,f_pop[0],f_pop[1],f_pop[2],f_pop[3],0);

  pobserver.outputSequences(0);
  ::sleep(500);
}

char Test::RNAc2cc(char rna)
{
  switch (rna) {
  case 'A':
    return 'U';
  case 'C':
    return 'G';
  case 'G':
    return 'C';
  case 'U':
    return 'A';
  default:
    std::cerr << "RNAcc2c: I got a non RNA character" << std::endl;
    std::exit(1);
  }
  return 0;
}

void Test::CmplSeq(const std::string &seq,std::string &cseq)
{
  cseq.clear();
  for(std::string::const_iterator iter = seq.begin();iter!=seq.end();iter++){
    cseq.push_back(Test::RNAc2cc(*iter));
  }
  reverse(cseq.begin(),cseq.end());
}
