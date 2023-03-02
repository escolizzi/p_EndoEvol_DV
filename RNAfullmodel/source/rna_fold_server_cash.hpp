#include <string>
#include <map>
#include <iostream>

#include "rna_fold.hpp"
#include "rna_fold_server.hpp"

#ifndef RNAFOLDSERVERCASH
#define RNAFOLDSERVERCASH




class RNAFoldServerCash : public RNAFoldServer {
  struct RNAData {
    std::string structure;
    double energy;
    RNAData(): structure(),energy(0.){}
    RNAData(const std::string &str,double ene): structure(str),energy(ene){}
  };
  typedef std::map<std::string, RNAFoldServerCash::RNAData> rna_map_type;
  typedef std::pair<rna_map_type::iterator, bool> rna_map_insert_result_type;
  
 private:
  rna_map_type fold_map;

 public:
  inline RNAFoldServerCash(const std::string &dist_method="C",const std::string &a_target_structure="");
  ~RNAFoldServerCash(){};

  inline double fold(const std::string &seq,std::string &str) ;
  /* Return the size of map */
  inline rna_map_type::size_type size() const;
  /* Clear the map (cashing data) */
  inline void clear();
  /* Insert (memorize) the folding */
  inline bool insert(const std::string &seq,const std::string &str,double ene);
};



RNAFoldServerCash::RNAFoldServerCash(const std::string &dist_method,const std::string &a_target_structure)
  : RNAFoldServer(dist_method,a_target_structure)
{}

double RNAFoldServerCash::fold(const std::string &seq,std::string &str)
{
  rna_map_insert_result_type result= fold_map.insert(rna_map_type::value_type(seq,RNAFoldServerCash::RNAData()));
  if(!result.second){
    str = result.first->second.structure;
    return result.first->second.energy;
  }else{
    result.first->second.energy = RNAFoldServer::fold(seq,str);
    result.first->second.structure = str;
    return result.first->second.energy;
  }
}

/* Return the size of map */
std::map<std::string, RNAFoldServerCash::RNAData>::size_type RNAFoldServerCash::size() const
{
  return fold_map.size();
}
/* Clear the map (cashing data) */
void RNAFoldServerCash::clear()
{
  fold_map.clear();
}
/* Insert (memorize) the folding */
bool RNAFoldServerCash::insert(const std::string &seq,const std::string &str,double ene)
{
  return fold_map.insert(rna_map_type::value_type(seq,RNAFoldServerCash::RNAData(str,ene))).second;
}


#endif //RNAFOLDSERVERCASH
