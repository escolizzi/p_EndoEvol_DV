#include <string>

#ifndef GENOTYPE
#define GENOTYPE
class Genotype {
  std::string p_seq;
  std::string p_str;
  std::string p_end3;
  std::string p_end5;
  unsigned int p_pop;

  std::string m_seq;
  std::string m_str;
  std::string m_end3;
  std::string m_end5;
  unsigned int m_pop;

 public:
  Genotype(const std::string &a_p_seq="",const std::string &a_p_str="",const std::string &a_p_end3="",const std::string &a_p_end5="",unsigned int a_p_pop=0,const std::string &a_m_seq="",const std::string &a_m_str="",const std::string &a_m_end3="",const std::string &a_m_end5="",unsigned int a_m_pop=0);

  bool operator<(const Genotype &rhs) const;
  bool operator==(const Genotype &rhs) const;
  bool operator>(const Genotype &rhs) const;

  const std::string &get_p_sequence() const;
  const std::string &get_p_structure() const;
  const std::string &get_p_end3() const;
  const std::string &get_p_end5() const;
  unsigned int get_p_pop() const;

  void set_p_sequence(const std::string &seq);
  void set_p_structure(const std::string &str);
  void set_p_end3(const std::string &end);
  void set_p_end5(const std::string &end);
  void set_p_pop(unsigned int pop);

  const std::string &get_m_sequence() const;
  const std::string &get_m_structure() const;
  const std::string &get_m_end3() const;
  const std::string &get_m_end5() const;
  unsigned int get_m_pop() const;

  void set_m_sequence(const std::string &seq);
  void set_m_structure(const std::string &str);
  void set_m_end3(const std::string &end);
  void set_m_end5(const std::string &end);
  void set_m_pop(unsigned int pop);
};
#endif // GENOTYPE
