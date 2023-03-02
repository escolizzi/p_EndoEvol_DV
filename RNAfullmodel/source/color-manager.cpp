#include <iostream>

#include "color-manager.hpp"
extern "C"{
#include "para.h"
}


ColorManager::ColorManager()
{
  for(unsigned int i=2;i<=::par_color_size;i++){
    color_list.push_back(ColorManager::ColorElement(i));
  }

  color_iter_vec.reserve(::par_color_size+1);
  for(std::list<ColorManager::ColorElement>::iterator iter=color_list.begin();iter!=color_list.end();iter++){
    /* color_iter_vec[0], [1] and [2] will point to
       color_list[0] which has color_value = 2. */
    if(iter==color_list.begin()){
      for(int i=0;i<3;i++)
	color_iter_vec.push_back(iter);
    }
    /* color_iter_vec[i] will point to color_list[i-2] which
       has color_balue = i.*/
    else{
      color_iter_vec.push_back(iter);
      /* debug */
      if(color_iter_vec[iter->getColorValue()]->getColorValue() != iter->getColorValue())
	std::cerr << "ColorManager::ColorManager() Error" << std::endl;
    }
  }
  used_border = color_list.end();
}

unsigned int ColorManager::getColor()
{

  /* The front element's color is the color to be returned. */
  unsigned int color = color_list.front().getColorValue();

  /* If 'used_border' points to the end of the 'color_list',
     there has not been an used color yet. But now, there is
     one used. Thus, used_border points to it. */
  if(used_border==color_list.end()){
    used_border = color_list.begin();
  }
  /* Furthermore, if all elements have been used already, and
     thus, 'used_border' is pointing to the
     color_list.begin(), 'used_border' should points to the
     next element, which is going to be the begining of the
     list after the current method call. */
  else if(used_border == color_list.begin()){
    used_border++;
  }
    
  /* Put a flag in this element to say that this color is
     used. */
  if(color_list.front().getIfUsed()){
    std::cerr << "ColorManager::getColor() Warning: Color use is overlapped.\n";
  }else{
    color_list.front().setIfUsed(true);
  }

  /* We rearrange the list. Take the element at the top of
     color_list, and inserted it at the end without destroying
     the iterator. */
  std::list<ColorManager::ColorElement> temp_list;
  temp_list.splice(temp_list.begin(),color_list,color_list.begin());
  color_list.splice(color_list.end(),temp_list,temp_list.begin());

  return color;
}

bool ColorManager::ifInRange(unsigned int color)
{
  return (color >= 2 && color <= ::par_color_size);
}

void ColorManager::returnColor(unsigned int color)
{
  if(!ColorManager::ifInRange(color)){
    std::cerr << "ColorManager::returnColor() Given returned color is out of range." << std::endl;
    return;
  }

  /* When a color is returned, we unflag the used-flag of that
     color element in the list, and place it in front of the
     color element to which 'used_border' is pointing to, so
     that this color will not be used for a while. This means
     that if the returned color is pointed by 'used_border',
     we have do increment the used_border after unflaging the
     used-fag instead of inserting the returned color in front
     of the border. */
  if(color_iter_vec[color]->getIfUsed()){
    color_iter_vec[color]->setIfUsed(false);
  }else{
    std::cerr << "ColorManager::returnColor() Given returned color has not been used." << std::endl;
    return;
  }

  if(color_iter_vec[color]!=used_border){
    std::list<ColorManager::ColorElement> temp_list;
    temp_list.splice(temp_list.begin(),color_list,color_iter_vec[color]);
    color_list.splice(used_border,temp_list,temp_list.begin());
  }else if(used_border!=color_list.end()){
    used_border++;
  }else{
    std::cerr << "ColorManager::returnColor() A bug." << std::endl;
    return;
  }
}
