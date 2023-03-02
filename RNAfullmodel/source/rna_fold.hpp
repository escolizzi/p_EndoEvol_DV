/*
  Class RNAFold

  General class of rna folding.
*/

#ifndef RNAFOLD
#define RNAFOLD
class RNAFold {

 public:
  inline RNAFold(){}
  virtual ~RNAFold(){}
  virtual double fold(const std::string &seq,std::string &str) =0;
  virtual int dist(const std::string &str) const =0;
  virtual int dist(const std::string &str1,const std::string &str2) const =0;
  virtual double eval(const std::string &sequence,const std::string &structure) const = 0;

};
#endif // RNAFOLD
