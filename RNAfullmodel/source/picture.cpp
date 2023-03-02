#include "display.hpp"

unsigned int Display::id_holder = 0;
int Display::margin = ::par_margin;

Display::Display()
{
  id = id_holder;
  id_holder++;
  view = ::New();
}

Display::~Display()
{
  ::PlaneFree(view);
}

