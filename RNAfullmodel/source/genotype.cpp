#include "genotype.hpp"

Genotype::Genotype(const std::string &a_p_seq,const std::string &a_p_str,const std::string &a_p_end3,const std::string &a_p_end5,unsigned int a_p_pop,const std::string &a_m_seq,const std::string &a_m_str,const std::string &a_m_end3,const std::string &a_m_end5,unsigned int a_m_pop)
  :  p_seq(a_p_seq),
     p_str(a_p_str),
     p_end3(a_p_end3),
     p_end5(a_p_end5),
     p_pop(a_p_pop),
     m_seq(a_m_seq),
     m_str(a_m_str),
     m_end3(a_m_end3),
     m_end5(a_m_end5),
     m_pop(a_m_pop)
{}


bool Genotype::operator<(const Genotype &rhs) const
{
  if(p_seq < rhs.p_seq)
    return true;
  else
    return false;
}

bool Genotype::operator==(const Genotype &rhs) const
{
  if(p_seq == rhs.p_seq)
    return true;
  else
    return false;
}

bool Genotype::operator>(const Genotype &rhs) const
{
  if(p_seq > rhs.p_seq)
    return true;
  else
    return false;
}

const std::string &Genotype::get_p_sequence() const
{
  return p_seq;
}

const std::string &Genotype::get_p_structure() const
{
  return p_str;
}

const std::string &Genotype::get_p_end3() const
{
  return p_end3;
}

const std::string &Genotype::get_p_end5() const
{
  return p_end5;
}

unsigned int Genotype::get_p_pop() const
{
  return p_pop;
}

const std::string &Genotype::get_m_sequence() const
{
  return m_seq;
}

const std::string &Genotype::get_m_structure() const
{
  return m_str;
}

const std::string &Genotype::get_m_end3() const
{
  return m_end3;
}

const std::string &Genotype::get_m_end5() const
{
  return m_end5;
}

unsigned int Genotype::get_m_pop() const
{
  return m_pop;
}

void Genotype::set_p_sequence(const std::string &seq)
{
  p_seq = seq;
}

void Genotype::set_p_structure(const std::string &str)
{
  p_str = str;
}

void Genotype::set_p_end3(const std::string &end)
{
  p_end3 = end;
}

void Genotype::set_p_end5(const std::string &end)
{
  p_end5 = end;
}

void Genotype::set_p_pop(unsigned int pop)
{
  p_pop = pop;
}

void Genotype::set_m_sequence(const std::string &seq)
{
  m_seq = seq;
}

void Genotype::set_m_structure(const std::string &str)
{
  m_str = str;
}

void Genotype::set_m_end3(const std::string &end)
{
  m_end3 = end;
}

void Genotype::set_m_end5(const std::string &end)
{
  m_end5 = end;
}

void Genotype::set_m_pop(unsigned int pop)
{
  m_pop = pop;
}
