/*
  ColorManager gives a color (from 2 to par_color_size). The
  color 1 is used as undefined phenotype (the phenotype which
  has never been in top something in its population). If there
  is no more color, it returns a color which has been used
  already. This means that if one assign too many color, then
  the color is not unique to the phenotype.

*/

#include <list>
#include <vector>

#ifndef COLORMANAGER
#define COLORMANAGER
class ColorManager{
  class ColorElement{
    unsigned int color_value;
    bool if_used;
  public:
    ColorElement(unsigned int color): color_value(color), if_used(false){}
    unsigned int getColorValue() const {return color_value;}
    bool getIfUsed() const {return if_used;}
    void setIfUsed(bool boolean) {if_used = boolean;}
  };

  std::list<ColorManager::ColorElement> color_list;

  /* 'color_iter_vec' speeds up the access to the element of
     the list which has the given color
     value. 'color_iter_vec[i]' will point to the element of
     the list which has the color_value=i. */
  std::vector<std::list<ColorManager::ColorElement>::iterator> color_iter_vec;

  /* The list of color is arranged such that the first part of
     the list contains colors which are not used. The later
     part of the list contains colors which are
     used. 'use_border' will point to the top used element.
     If there is no color used, 'use_border' will point to
     color_list.end().

     When getColor() is called, the color in the head of the
     list will be returned and the element in the head of the
     list will be placed in the end of the list. When
     returnColor() is called, the color element of that color
     is inserted in the border of used-color and
     not-used-color. */
  std::list<ColorManager::ColorElement>::iterator used_border;

 public:
  ColorManager();
  unsigned int getColor();
  void returnColor(unsigned int color);

 private:
  /* Return true if the color is in the range */
  bool ifInRange(unsigned int color);
};

#endif  //COLORMANAGER
