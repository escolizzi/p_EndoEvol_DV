#include <string>
#include <cstring>

#include "rna_fold_lib.hpp"
#include "rna_fold_lib_cash.hpp"

RNAFoldLibCash::RNAFoldLibCash(const std::string &a_target_structure)
  : RNAFoldLib(a_target_structure)
{}

double RNAFoldLibCash::fold(const std::string &seq,std::string &str)
{
  rna_map_insert_result_type result= fold_map.insert(rna_map_type::value_type(seq,RNAFoldLibCash::RNAData()));
  if(!result.second){
    str = result.first->second.structure;
    return result.first->second.energy;
  }else{
    result.first->second.energy = RNAFoldLib::fold(seq,str);
    result.first->second.structure = str;
    return result.first->second.energy;
  }
}

double RNAFoldLibCash::fold(char *sequence,char *structure)
{
  std::string seq(sequence);

  rna_map_insert_result_type result= fold_map.insert(rna_map_type::value_type(seq,RNAFoldLibCash::RNAData()));
  if(!result.second){
    std::strcpy(structure,result.first->second.structure.c_str());
    return result.first->second.energy;
  }else{
    result.first->second.energy = RNAFoldLib::fold(sequence,structure);
    result.first->second.structure = structure;
    return result.first->second.energy;
  }
}


/* Return the size of map */
std::map<std::string, RNAFoldLibCash::RNAData>::size_type RNAFoldLibCash::size() const
{
  return fold_map.size();
}
/* Clear the map (cashing data) */
void RNAFoldLibCash::clear()
{
  fold_map.clear();
}
/* Insert (memorize) the folding */
bool RNAFoldLibCash::insert(const std::string &seq,const std::string &str,double ene)
{
  return fold_map.insert(rna_map_type::value_type(seq,RNAFoldLibCash::RNAData(str,ene))).second;
}
