#include "phenotype.hpp"

Phenotype::Phenotype(const std::string &a_p_str,const std::string &a_p_end3,const std::string &a_p_end5,const std::string &a_m_str,const std::string &a_m_end3,const std::string &a_m_end5)
  : p_str(a_p_str),p_end3(a_p_end3),p_end5(a_p_end5),m_str(a_m_str),m_end3(a_m_end3),m_end5(a_m_end5)
{}

bool Phenotype::operator<(const Phenotype &rhs) const
{
  if(p_str < rhs.p_str)
    return true;
  else if(p_str > rhs.p_str)
    return false;

  if(p_end3 < rhs.p_end3)
    return true;
  else if(p_end3 > rhs.p_end3)
    return false;

  if(p_end5 < rhs.p_end5)
    return true;
  else if(p_end5 > rhs.p_end5)
    return false;

  if(m_str < rhs.m_str)
    return true;
  else if(m_str > rhs.m_str)
    return false;

  if(m_end3 < rhs.m_end3)
    return true;
  else if(m_end3 > rhs.m_end3)
    return false;

  if(m_end5 < rhs.m_end5)
    return true;
  else if(m_end5 > rhs.m_end5)
    return false;

  return false;
}

bool Phenotype::operator>(const Phenotype &rhs) const
{
  if(p_str > rhs.p_str)
    return true;
  else if(p_str < rhs.p_str)
    return false;

  if(p_end3 > rhs.p_end3)
    return true;
  else if(p_end3 < rhs.p_end3)
    return false;

  if(p_end5 > rhs.p_end5)
    return true;
  else if(p_end5 < rhs.p_end5)
    return false;

  if(m_str > rhs.m_str)
    return true;
  else if(m_str < rhs.m_str)
    return false;

  if(m_end3 > rhs.m_end3)
    return true;
  else if(m_end3 < rhs.m_end3)
    return false;

  if(m_end5 > rhs.m_end5)
    return true;
  else if(m_end5 < rhs.m_end5)
    return false;

  return false;
}

bool Phenotype::operator==(const Phenotype &rhs) const
{
  if(p_str != rhs.p_str)
    return false;
  else if(p_end3 != rhs.p_end3)
    return false;
  else if(p_end5 != rhs.p_end5)
    return false;
  else if(m_str != rhs.m_str)
    return false;
  else if(m_end3 != rhs.m_end3)
    return false;
  else if(m_end5 != rhs.m_end5)
    return false;
  else
    return true;
}

void Phenotype::set_p_str(const std::string &a_p_str)
{
  p_str = a_p_str;
}

void Phenotype::set_p_end3(const std::string &a_p_end3)
{
  p_end3 = a_p_end3;
}

void Phenotype::set_p_end5(const std::string &a_p_end5)
{
  p_end5 = a_p_end5;
}

void Phenotype::set_m_str(const std::string &a_m_str)
{
  m_str = a_m_str;
}

void Phenotype::set_m_end3(const std::string &a_m_end3)
{
  m_end3 = a_m_end3;
}

void Phenotype::set_m_end5(const std::string &a_m_end5)
{
  m_end5 = a_m_end5;
}

const std::string &Phenotype::get_p_str() const
{
  return p_str;
}
const std::string &Phenotype::get_p_end3() const
{
  return p_end3;
}
const std::string &Phenotype::get_p_end5() const
{
  return p_end5;
}

const std::string &Phenotype::get_m_str() const
{
  return m_str;
}
const std::string &Phenotype::get_m_end3() const
{
  return m_end3;
}
const std::string &Phenotype::get_m_end5() const
{
  return m_end5;
}
