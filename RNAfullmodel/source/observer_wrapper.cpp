extern "C"{
#include "cash2.h"
}
#include "observer.hpp"

namespace ObserverWrapper {
  Observer *observer;
}

extern "C" void ObserverWrapper_main(int time,const TYPE2 **world, struct point ***neighbor)
{
  ObserverWrapper::observer->main(time,world, neighbor);
}

extern "C" void ObserverWrapper_init()
{
  static  Observer ob;
  ObserverWrapper::observer = &ob;
}
