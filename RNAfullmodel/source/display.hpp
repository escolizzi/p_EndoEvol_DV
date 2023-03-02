/* 
   Call this class after making nrow and ncol correct.
*/

typedef int TYPE;
extern "C" {
  int PlaneDisplay(TYPE**,int,int,TYPE);
  int Mouse();
}

#ifndef DISPLAY
#define DISPLAY


extern int nrow,ncol;

class Display{

  int margin;
  TYPE **view_func;
  TYPE **view_phen;
  TYPE **view_movie;

  int nrow_mem,ncol_mem;

 public:
  Display();
  ~Display();
  inline void set_func(int nr,int nc,unsigned int color);
  inline void set_phen(int nr,int nc,unsigned int color);
  inline void draw_display();
  void draw_movie();
  inline int mouse();

 private:
  Display(const Display &rhs){}
  void operator=(const Display &rhs){}
};

void Display::set_func(int nr,int nc,unsigned int color)
{
  view_func[nr][nc] = (TYPE) color;
}

void Display::set_phen(int nr,int nc,unsigned int color)
{
  view_phen[nr][nc] = (TYPE) color;
}

void Display::draw_display()
{
  ::PlaneDisplay(view_func,margin,margin+0*(ncol_mem+margin),0);
  ::PlaneDisplay(view_phen,margin,margin+1*(ncol_mem+margin),0);
}

int Display::mouse()
{
  return ::Mouse();  
}
#endif //VIEW
