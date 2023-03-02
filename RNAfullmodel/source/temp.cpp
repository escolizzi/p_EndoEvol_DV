#include <iostream>
#include <string>

void print_print(const std::string &msg)
{
  std::string mess = msg;
  std::cout << mess << std::endl;
}

void print(const std::string &msg)
{
  print_print(msg);
}

int main()
{
  char msg[100]="a";

  print(msg);
  return 0;
}
