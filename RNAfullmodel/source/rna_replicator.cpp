#include <cstring>

#include "rna_replicator.hpp"

unsigned int RNAreplicator::max_match(const char *a_end3,const char *a_end5) const
{
  int i,j,len3,len5,len_diff;
  int nmatch; /* current */
  int pnmatch=0; /* previous */
  const char *end_last,*end_middle; /* dont be confused by the name. */

  len3=strlen(a_end3);
  len5=strlen(a_end5);

  if(len3==0 || len5==0)
    return 0;

  if(len3>=len5) {
    len_diff = len3 - len5;
    end_last = a_end5 + len5;
    end_middle = a_end3 + len_diff;
    /*
      Let the length of 3' be 'll' and 5' be 'dd'.
      Let l = ll-1 and d = dd-1.
      "end_last" is pointing to "5[dd]".
      (i=1)
      3'       012........l   -> 3[0] is the begining
      5' 012...d              -> 5[dd-i] is the begining

      (i=dd)
      3' 0123........l        -> 3[0]
      5' 0123.....d           -> 5[dd-dd]
    */
    for(i=1,j=len5;i<=j;i++){
      nmatch = RNAreplicator::n_base_pair(a_end3,end_last-i,i);
      if(pnmatch<nmatch)
	pnmatch = nmatch;
    }
    /*
      (i=1)
      3' 0123.........l     -> 3[i]
      5'  0123....d         -> 5[0]

      (i=ll-dd)
      3' 0123.........l     -> 3[(ll-dd)]
      5'      0123....d     -> 5[0]
    */
    for(i=1,j=len_diff;i<=j;i++){
      nmatch = RNAreplicator::n_base_pair(a_end3+i,a_end5,len5);
      if(pnmatch<nmatch)
	pnmatch = nmatch;
    }
    /*
      Let "end_middle" point to "3[(ll-dd)]".
      (i=1)
      3' 0123.........l      -> 3[(ll-dd)+i]
      5'       0123....d     -> 5[0]

      (i=dd-1)
      3' 0123........l            -> 3[(ll-dd)+dd-1] = 3[l]
      5'             0123.....d   -> 5[0]
    */
    for(i=1,j=len5-1;i<=j;i++){
      nmatch = RNAreplicator::n_base_pair(end_middle+i,a_end5,len5-i);
      if(pnmatch<nmatch)
	pnmatch = nmatch;
    }
    return pnmatch;
  }
  else {
    len_diff = len5 - len3;
    end_last = a_end3 + len3;
    end_middle = a_end5 + len_diff;
    for(i=1,j=len3;i<=j;i++){
      nmatch = RNAreplicator::n_base_pair(a_end5,end_last-i,i);
      if(pnmatch<nmatch)
	pnmatch = nmatch;
    }
    for(i=1,j=len_diff;i<=j;i++){
      nmatch = RNAreplicator::n_base_pair(a_end5+i,a_end3,len3);
      if(pnmatch<nmatch)
	pnmatch = nmatch;
    }
    for(i=1,j=len3-1;i<=j;i++){
      nmatch = RNAreplicator::n_base_pair(end_middle+i,a_end3,len3-i);
      if(pnmatch<nmatch)
	pnmatch = nmatch;
    }
  }
  return pnmatch;
}
