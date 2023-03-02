#include <stdio.h>
#include <strings.h>
#include <png.h>
#include "cash.h"

int nRow, nCol;
extern int userCol[256][4];
extern static unsigned char *data;

extern static png_structp png_ptr;
extern static png_infop info_ptr;
extern static png_bytep ptr;
extern static int nframes = 0;

extern static char dirname[512];


int PlanePNG(TYPE **a, int c)
{
  int i,j;
  char name[512];
  FILE *fp;
  sprintf(name,"%s/%.5d.png",dirname,nframes);
  fp = fopen(name,"wb");
  png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING,
   (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
  info_ptr = png_create_info_struct (png_ptr);
  png_init_io(png_ptr, fp);
  png_set_IHDR(png_ptr, info_ptr, nCol, nRow,
    8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
    PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
  png_write_info(png_ptr,info_ptr);
  for (i=0; i < nRow; i++) {
    for (j=0; j < nCol; j++) {
      data[i*3*nCol + j*3] = userCol[c+a[i+1][j+1]][1];
      data[i*3*nCol + j*3 + 1] = userCol[c+a[i+1][j+1]][2];
      data[i*3*nCol + j*3 + 2] = userCol[c+a[i+1][j+1]][3];
    }
    ptr = data + i*3*nCol;
    png_write_rows(png_ptr, &ptr, 1);
  }
  png_write_end(png_ptr, info_ptr);
  png_destroy_write_struct(&png_ptr,(png_infopp)NULL);
  fclose(fp);
  nframes++;
  return (0);
}
