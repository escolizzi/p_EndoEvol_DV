/* 
   Call this class after making nrow and ncol correct.
*/

extern "C" {
#include "cash.h"
}

#ifndef PICTURE
#define PICTURE
class Picture{
  /* Total number of object ever created */
  static unsigned int id_holder;

  TYPE **view;
  unsigned int id;

 public:
  Picture();
  ~Picture();
  inline void set(int nr,int nc,unsigned int color);
  void draw();

 private:
  Picture(const Picture &rhs){}
  const Picture &operator=(const Picture &rhs){}
}

void Picture::set(int nr,int nc,unsigned int color)
{
  view[nr][nc] = (TYPE) color;
}
#endif //VIEW
