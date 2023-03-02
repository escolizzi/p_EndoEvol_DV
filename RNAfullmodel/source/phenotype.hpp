#include <string>

#ifndef PHENOTYPE
#define PHENOTYPE
class Phenotype{
  std::string p_str;
  std::string p_end3;
  std::string p_end5;

  std::string m_str;
  std::string m_end3;
  std::string m_end5;

 public:
  Phenotype(){}
  Phenotype(const std::string &a_p_str,const std::string &a_p_end3,const std::string &a_p_end5,const std::string &a_m_str,const std::string &a_m_end3,const std::string &a_m_end5);

  bool operator<(const Phenotype &rhs) const;
  bool operator>(const Phenotype &rhs) const;
  bool operator==(const Phenotype &rhs) const;

  void set_p_str(const std::string &a_p_str);
  void set_p_end3(const std::string &a_p_end3);
  void set_p_end5(const std::string &a_p_end5);

  void set_m_str(const std::string &a_m_str);
  void set_m_end3(const std::string &a_m_end3);
  void set_m_end5(const std::string &a_m_end5);

  const std::string &get_p_str() const;
  const std::string &get_p_end3() const;
  const std::string &get_p_end5() const;

  const std::string &get_m_str() const;
  const std::string &get_m_end3() const;
  const std::string &get_m_end5() const;

};

#endif // PHENOTYPE
