#include <iostream>

extern "C" {
#include "para.h"
}
#include "display.hpp"

extern "C" {
  /* X */
  int PlaneDisplay(TYPE**,int,int,TYPE);
  TYPE **New();
  int OpenDisplay(char*,int,int);
  int PlaneFree(TYPE**);
  int CloseDisplay();
  int Mouse();

  /* PNG */
  int PlanePNG(TYPE **a, int c);
  int OpenPNG(char name[],int nrow,int ncol);
}

extern int nrow;
extern int ncol;

Display::Display()
  : margin(::par_margin), nrow_mem(::nrow), ncol_mem(::ncol)
{
  view_func = ::New();
  view_phen = ::New();

  ::ncol = ::ncol*2 + 3*margin;
  ::nrow = ::nrow + 2*margin;
  view_movie = ::New();

  ::ncol = ncol_mem;
  ::nrow = nrow_mem;

  if(::par_if_display == 1)
    ::OpenDisplay("Prototype",::nrow+2*margin,2*::ncol+(1+2)*margin);
  if(::par_if_movie == 1)
    ::OpenPNG(::par_movie_directory_name, ::nrow + 2*margin, 2*::ncol+3*margin);
}

Display::~Display()
{
  ::PlaneFree(view_func);
  view_func = NULL;
  ::PlaneFree(view_phen);
  view_phen = NULL;
  ::PlaneFree(view_movie);
  view_movie = NULL;

  if(par_if_display==1)
  ::CloseDisplay();
}

void Display::draw_movie()
{

  for(int i=1;i<=nrow_mem;i++)
    for(int j=1;j<=ncol_mem;j++){
      view_movie[i+margin][j+margin] = view_func[i][j];
      view_movie[i+margin][ncol_mem+2*margin+j] = view_phen[i][j];
    }

  ::PlanePNG(view_movie,0);
}
