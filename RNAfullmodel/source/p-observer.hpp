/*
  setAllPopZero();
  Gobserver.communicatePobserver(*this);
  makeRanking(); <- also deletes zero pulation phenotype.
  .... output ....

*/

#include <map>
#include <set>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <cstdlib>
#include <cstring>
/* You see? NEVER EVER USE MACRO! */
#ifdef C
#undef C
#endif
#include <boost/tuple/tuple_comparison.hpp>

extern "C" {
#include <RNAstruct.h>
}
extern "C"{
#include "para.h"
}

extern "C" {
#include "cash2.h"
}

#include "phenotype.hpp"
#include "color-manager.hpp"

#ifndef POBSERVER
#define POBSERVER
class Pobserver{
  struct Population{
  private:
    unsigned int p_pop;
    std::string p_master;
    unsigned int p_master_pop;
    bool p_if_catalyst;

    unsigned int m_pop;
    std::string m_master;
    unsigned int m_master_pop;
    bool m_if_catalyst;

    unsigned int color; // default = 1 (white)

  public:
    Population():p_pop(0),p_master(),p_master_pop(0),p_if_catalyst(false),m_pop(0),m_master(),m_master_pop(0),m_if_catalyst(false),color(1){}
    unsigned int get_p_pop()const{return p_pop;}
    const std::string &get_p_master()const{return p_master;}
    unsigned int get_p_master_pop()const{return p_master_pop;}
    bool get_p_if_catalyst()const{return p_if_catalyst;}

    unsigned int get_m_pop()const{return m_pop;}
    const std::string &get_m_master()const{return m_master;}
    unsigned int get_m_master_pop()const{return m_master_pop;}
    bool get_m_if_catalyst()const{return m_if_catalyst;}

    unsigned int get_color()const{return color;}

    void set_p_pop(unsigned int a){p_pop=a;}
    void set_p_master(const std::string &a){p_master=a;}
    void set_p_master_pop(unsigned int a){p_master_pop=a;}
    void set_p_if_catalyst(bool a){p_if_catalyst=a;}

    void set_m_pop(unsigned int a){m_pop=a;}
    void set_m_master(const std::string &a){m_master=a;}
    void set_m_master_pop(unsigned int a){m_master_pop=a;}
    void set_m_if_catalyst(bool a){m_if_catalyst=a;}

    void set_color(unsigned int a){color=a;}
  };

  typedef std::map<Phenotype,Pobserver::Population> tree_type;

  /* Binary tree data where all the phenotypes are. */
  tree_type tree;

  /* Vector which has iterators to tree in population
     descending order. Call 'decideColor_deleteZeroPop()' to
     update the vector. */
  std::vector<tree_type::iterator> tree_iter_vec;
  
  ColorManager color_manager;

  std::string target_structure;

  unsigned int undefined_pop;

  /* This is used to get the color index defined by main.cpp
     from an RGB value. Use getColorIndex() to get the index
     of color. */
  std::map<const boost::tuple<int,int,int>,int> color_map;

 public:
  Pobserver();
  ~Pobserver(){}

  /* Insert the genotype into the phenotype tree. Phenotype
     has plus and minus as genotype. In contrast to genotype,
     the plus and minus of phenotype is determined by < on
     course grained structure and open end3 and end5 in this
     order. This means that the plus and minus of genotype
     does not correspond to the plus and minus of
     phenotype. */
  void insert(const std::string &seq_1,const std::string &str_1,const std::string &end3_1,const std::string &end5_1,unsigned int pop_1,const std::string &seq_2,const std::string &str_2,const std::string &end3_2,const std::string &end5_2,unsigned int pop_2);
  void getEverything();
  /* Set all population and master sequences to zero. Color
     will not be set to zero. This method is called to prepare
     for inserting a new set of genotypes. */
  void setAllPopZero();
  /* Sort the phenotype by population, delete the phenotype
     with no population and report the unused color. And then,
     put the color. Along with that, the mothod renew
     'tree_iter_vec'. This is needed before outputing
     sequences or plots etc. */
  void makeRanking();
  /* The method will put population in the parameter
     vector. The first vector will get the population in five
     categories: [0] symmetric catalyst; [1] asymmetric
     catalyst with function; [2] asymmetric catalyst without
     function; [3] parasites; [4] undefined population. The
     second-fourth vectors are for the population of each
     phenotype. The same index corresponds to the same
     phenotype. 'Color_vec' specifies the color of
     phenotype. 'P_population' is the population of plus
     phenotype. 'M_population' is the population of minus
     phenotype. 'If_catalyst' specifies whether this phenotype
     is catalyst or not. The distinction between symmetric and
     asymmetric catalyst is not made. */
  void getPopulation(std::vector<int> &functional_pop,std::vector<unsigned int> &color_vec,std::vector<int> &p_population,std::vector<int> &m_population,std::vector<bool> &if_catalyst) const;
  /* Output sequences */
  void outputSequences(unsigned int time) const;
  /* Get a color of phenotype */
  void getColor(unsigned int &color_func,unsigned int &color_phen,const std::string &sequence,const std::string &structure,const std::string &end3,const std::string &end5,const std::string &c_structure) const;
  void getColorByComposition(int i,int j,unsigned int &color_comp_cat,unsigned int &color_comp_par,const std::string &sequence,const std::string &structure,const std::string &end3,const std::string &end5,const std::string &c_structure, struct point ***neighbor,const TYPE2 **world) const;
  inline void setUndefinedPop(unsigned int pop);

  void debug();

 private:
  bool if1stIsPlus(const std::string &str_1,const std::string &end3_1,const std::string &end5_1,const std::string &str_2,const std::string &end3_2,const std::string &end5_2) const;
  void full_str2coarse_str(std::string &course_structure,const std::string &structure) const;
  struct LartherThanByPop: public std::binary_function<tree_type::const_iterator,tree_type::const_iterator,bool>{
    bool operator()(const tree_type::const_iterator &x,const tree_type::const_iterator &y) const
    {
      return ((x->second.get_p_pop()+x->second.get_m_pop()) > (y->second.get_p_pop()+y->second.get_m_pop()));
    }
  };
  int maxbasepair(const std::string &end3)const;
  inline char RNAc2cc(char rna) const; /* RNA interger to complementary RNA character */
  inline void getCmplSeq(const std::string &seq,std::string &cseq) const;
  inline void countACGU(int count[4],const std::string& end) const;
  inline int getColorIndex(int r,int g,int b) const;
};

char Pobserver::RNAc2cc(char rna) const
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

void Pobserver::getCmplSeq(const std::string &seq,std::string &cseq) const
{
  if(!cseq.empty())
    cseq.clear();
  std::string::const_iterator end_iter = seq.end();
  for(std::string::const_iterator iter = seq.begin();iter!=end_iter;iter++){
    cseq.push_back(Pobserver::RNAc2cc(*iter)); 
  }
  
  /* Convert the 3' and 5' order */
  reverse(cseq.begin(),cseq.end());
}

void Pobserver::setUndefinedPop(unsigned int pop)
{
  Pobserver::undefined_pop = pop;
}

void Pobserver::countACGU(int count[4],const std::string& terminal) const
{
  count[0] = count[1] = count[2] = count[3] = 0;

  for(std::string::const_iterator iter = terminal.begin(); iter!=terminal.end();iter++){
    switch(*iter)
      {
      case 'A': count[0]++; break;
      case 'C': count[1]++; break;
      case 'G': count[2]++; break;
      case 'U': count[3]++; break;
      default: break;
      }
  }
}

inline int Pobserver::getColorIndex(int r,int g,int b) const
{
  std::map<const boost::tuple<int,int,int>,int>::const_iterator iter = color_map.find(boost::tuple<int,int,int>(r,g,b));
  if(iter!=color_map.end()){
    return iter->second;
  }else{
    std::cerr << "Pobserver::getColorIndex() Got an RGB value not in color-table." << std::endl;
    std::cerr << r << " " << g << " " << b << std::endl;
    std::exit(-1);
  }
  return 0;
}
#endif // POBSERVER
