#include <iostream>
#include <map>
#include <string>
#include <algorithm>
#include <cstdlib>

#include "g-observer.hpp"

void Gobserver::insert(const std::string &sequence,const std::string &structure,const std::string &end3,const std::string &end5,const std::string &csequence,const std::string &cstructure,const std::string &cend3,const std::string &cend5, const double &kcat)
{
  /* Empty strings in the 'sequence' arguments are not
     allowed */
  if(sequence.empty()){
    std::cerr << "Gobserver::insert() Try to insert an empty string." << std::endl;
    std::exit(0);
  }

  /* If structures are not given, the sequence's property is
     not determined. We do not insert. Note that end3 and end5
     can be an empty string because not all RNA replicator
     keeps its end open. */
  if(structure.empty()||cstructure.empty()){
    undetermined_pop++;
    return;
  }

  /*There is no need for this, as sequences are already entirely known*/
  /* We make the complementary sequence of the given
     'sequence', and then determine their end3 and end5. */
  //std::string complementary;
  //Gobserver::getCmplSeq(sequence,complementary);

  //std::string c_end5(complementary.substr(c_structure.rfind(")")+1,std::string::npos));
  //reverse(c_end5.begin(),c_end5.end());

  /* Rest is done by the other method. */
  Gobserver::insert(sequence,structure,end3,end5,csequence,cstructure,cend3,cend5);
}

void Gobserver::insert(const std::string &sequence,const std::string &structure,const std::string &end3,const std::string &end5,const std::string &c_sequence,const std::string &c_structure,const std::string &c_end3,const std::string &c_end5)
{
    /* The plus and minus of sequence is determined by the
     string comparison with '>' */
  /* If 'sequence' is plus-string */
  if(Gobserver::if1stIsPlusString(sequence,c_sequence)){
    /* Try insert */    
    std::pair<tree_type::iterator,bool> result = tree.insert(std::pair<std::string,Genotype>(sequence,Genotype()));
    
    /* If the node has not existed yet, we set the node
       properly. Otherwise, we only increment the population
       size. */
    if(result.second){
      /* Set plus-string */
      result.first->second.set_p_sequence(sequence);
      result.first->second.set_p_structure(structure);
      result.first->second.set_p_end3(end3);
      result.first->second.set_p_end5(end5);
      
      /* Set minus-string */
      result.first->second.set_m_sequence(c_sequence);
      result.first->second.set_m_structure(c_structure);
      result.first->second.set_m_end3(c_end3);
      result.first->second.set_m_end5(c_end5);
    }

    /* 'sequence' is the individual we want to increment its
       population size. And, 'sequence' is the plus-string of
       this genotype. Thus we increment the plus-string
       population  */
    result.first->second.set_p_pop(result.first->second.get_p_pop()+1);
  }
  /* If 'sequence' is minus-string */
  else{
    /* Try insert */    
    std::pair<tree_type::iterator,bool> result = tree.insert(std::pair<std::string,Genotype>(c_sequence,Genotype()));

    if(result.second){
      /* Set minus-string */
      result.first->second.set_m_sequence(sequence);
      result.first->second.set_m_structure(structure);
      result.first->second.set_m_end3(end3);
      result.first->second.set_m_end5(end5);
      
      /* Set plus-string */
      result.first->second.set_p_sequence(c_sequence);
      result.first->second.set_p_structure(c_structure);
      result.first->second.set_p_end3(c_end3);
      result.first->second.set_p_end5(c_end5);
    }

    /* 'sequence' is minus-string. */
    result.first->second.set_m_pop(result.first->second.get_m_pop()+1);
  }
}

unsigned int Gobserver::getPop(const std::string &sequence) const
{
  if(sequence.empty())
    return undetermined_pop;

  std::string csequence;
  Gobserver::getCmplSeq(sequence,csequence);

  const tree_type::const_iterator end_iter = tree.end();

  if(Gobserver::if1stIsPlusString(sequence,csequence)){
    tree_type::const_iterator iter= tree.find(sequence);
    if(iter != end_iter){
      return iter->second.get_p_pop();
    }else{
      return 0;
    }
  }else{
    tree_type::const_iterator iter= tree.find(csequence);
    if(iter != end_iter){
      return iter->second.get_m_pop();
    }else{
      return 0;
    }
  }
}

void Gobserver::deleteZeroPopGenotype()
{
  tree_type::iterator iter = tree.begin();
  const tree_type::const_iterator end_iter = tree.end();
  tree_type::iterator prev_iter;

  while(iter!=end_iter){
    /* We may delete 'iter', but that destroys 'iter'. So, we
       copy it in prev_iter and destroy 'prev_iter' if
       necessarily. */
    prev_iter = iter;
    ++iter;

    /* If the populations of both plus- and minus-string are
       zero, we delete this node. */
    if(prev_iter->second.get_p_pop()==0 && prev_iter->second.get_m_pop()==0){
      tree.erase(prev_iter);
    }
  }
}

void Gobserver::setAllPopZero()
{
  const tree_type::const_iterator end_iter = tree.end();
  for(tree_type::iterator iter = tree.begin();iter!=end_iter;iter++){
    iter->second.set_p_pop(0);
    iter->second.set_m_pop(0);
  }
  undetermined_pop = 0;
}

void Gobserver::clearAll()
{
  tree.clear();
}

void Gobserver::communicatePobserver(Pobserver &pobserver)
{
  const tree_type::const_iterator end_iter = tree.end();
  for(tree_type::iterator iter = tree.begin();iter!=end_iter;iter++){
    pobserver.insert(iter->second.get_p_sequence(),iter->second.get_p_structure(),iter->second.get_p_end3(),iter->second.get_p_end5(),iter->second.get_p_pop(),iter->second.get_m_sequence(),iter->second.get_m_structure(),iter->second.get_m_end3(),iter->second.get_m_end5(),iter->second.get_m_pop());
  }
  
  pobserver.setUndefinedPop(undetermined_pop);
}

void Gobserver::dump()
{
  const tree_type::const_iterator end_iter = tree.end();
  for(tree_type::iterator iter = tree.begin();iter!=end_iter;iter++){
    std::cout << iter->second.get_p_sequence() << std::endl;
    std::cout << "pop=" << iter->second.get_p_pop() << std::endl;
    std::cout << iter->second.get_m_sequence() << std::endl;
    std::cout << "pop=" << iter->second.get_m_pop() << std::endl;
    std::cout << "---------------------------------"  << std::endl;
  }
}
