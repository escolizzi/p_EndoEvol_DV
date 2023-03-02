#include "p-observer.hpp"
#include "g-observer.hpp"
#include "plot_grace.hpp"
#include "display.hpp"

extern "C" {
#include "cash2.h"
}

#ifndef OBSERVER
#define OBSERVER

class Observer {
  Gobserver g_observer;
  Pobserver p_observer;
  PlotGrace plot_grace;
  Display display;

 public:
  Observer(){}
  ~Observer(){}
  void main(int time,const TYPE2 **world, struct point ***neighbor);
  inline int mouse();

 private:
  void observe(const TYPE2 **world);
  void plotPopulation(int time);
  void outputSequences(int time);
  void updateDisplayMovieColorByComposition(const TYPE2 **world, struct point ***neighbor);
};

int Observer::mouse()
{
  return display.mouse();
}

#endif //OBSERVER
